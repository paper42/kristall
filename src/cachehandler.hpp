#ifndef CACHEHANDLER_HPP
#define CACHEHANDLER_HPP

#include "mimeparser.hpp"
#include <memory>
#include <unordered_map>

#include <QUrl>
#include <QString>
#include <QByteArray>
#include <QtGlobal>

// Need a QString hash implementation for Qt versions below 5.14
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <QHash>
namespace std
{
    template<>
    struct hash<QString>
    {
        std::size_t operator()(const QString& s) const noexcept
        {
            return (size_t)qHash(s);
        }
    };
}
#endif

struct CachedPage
{
    QUrl url;

    QByteArray body;

    MimeType mime;

    int scroll_pos;

    // also: maybe compress page contents? May test
    // to see if it's worth it

    CachedPage(const QUrl &url, const QByteArray &body, const MimeType &mime)
        : url(url), body(body), mime(mime), scroll_pos(-1)
    {}
};

typedef std::unordered_map<QString, std::shared_ptr<CachedPage>> CacheMap;

class CacheHandler
{
public:
    void push(QUrl const & url, QByteArray const & body, MimeType const & mime);

    std::shared_ptr<CachedPage> find(QUrl const &url);

    bool contains(QUrl const & url) const;

    CacheMap const& getPages() const;

private:
    std::shared_ptr<CachedPage> find(QString const &url);

    bool contains(QString const & url) const;

private:
    // In-memory cache storage.
    CacheMap page_cache;
};

#endif
