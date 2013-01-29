#include "jsapi.h"

#include <QDebug>

#include "bookmarks.h"
#include "xbel.h"
#include "browserapplication.h"

void JsAPI::addTop8(const QString &url, const QString &title)
{
    BookmarksManager *manager = BrowserApplication::bookmarksManager();
    BookmarkNode *node = new BookmarkNode(BookmarkNode::Bookmark);

    node->title = title;
    node->url = url;

    manager->addBookmark(manager->top8(), node );
    qDebug() << "Added";
}
void JsAPI::test()
{
    qDebug() << "JsAPI";
}
