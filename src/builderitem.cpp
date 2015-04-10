#include "builderitem.h"

builderItem::builderItem(QListWidget *list_in)
{
    list = list_in;

}

builderItem::~builderItem()
{

}

void builderItem::setCardInfo(QString titlein, QString frontin, QString backin){
    card_title = titlein;
    front_text = frontin;
    back_text = backin;
}

QString builderItem::ftext(){
    return front_text;
}

QString builderItem::btext(){
    return back_text;
}