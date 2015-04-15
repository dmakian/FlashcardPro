#include "user.h"
#include "decksview.h"

User::User(QString usrn_in, QString pw_in, QString fn_in, QString ln_in, std::vector<Deck*> decks_in)
{
    username = usrn_in;
    password = pw_in;
    first_name = fn_in;
    last_name = ln_in;
    decks= decks_in;
}

User::~User(){}

bool User::sameName(QString check)
{
    if(check == username) return true;
    return false;
}

void User::goToDecksView(QStackedWidget* pages_in)
{
    int index = pages_in->addWidget(new DecksView(pages_in));
    pages_in->setCurrentIndex(index);
}
