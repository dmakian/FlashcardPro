#include "deckmenu.h"
#include <QtWidgets>
#include "studyarea.h"
#include "statsview.h"
#include "builder.h"

DeckMenu::DeckMenu(Deck* _deck,QStackedWidget* pages_in, QWidget *parent)
{
    pages = pages_in;
    deck = _deck;

    QVBoxLayout *vbox = new QVBoxLayout;

    QFont naxa;
    QFontDatabase db;
    naxa = db.font("Nexa Bold","Normal",40);

    _deck->menu = this;

    title = new QLabel(deck->deck_name);
    title->setFont(naxa);
    vbox->addWidget(title);
    title->setAlignment(Qt::AlignCenter);

    QPushButton *study = new QPushButton("Study Deck");
    vbox->addWidget(study);
    connect(study,SIGNAL(clicked()),this,SLOT(study()));

    QPushButton *edit = new QPushButton("Edit Deck");
    vbox->addWidget(edit);
    connect(edit,SIGNAL(clicked()), this, SLOT(edit()));

    QPushButton *stats = new QPushButton("View Deck Statistics");
    vbox->addWidget(stats);
    connect(stats,SIGNAL(clicked()),this,SLOT(stats()));

    QPushButton *save = new QPushButton("Save Deck Progress");
    vbox->addWidget(save);
    connect(save, SIGNAL(clicked()), this, SLOT(saveDeckCallback()));

    QPushButton *back = new QPushButton("Back to Decks");
    vbox->addWidget(back);
    connect(back,SIGNAL(clicked()),this,SLOT(back()));

    this->setLayout(vbox);
}

DeckMenu::~DeckMenu()
{

}

void DeckMenu::study()
{
    StudyArea* area = new StudyArea(deck, pages);
    int index = pages->addWidget(area);
    pages->setCurrentIndex(index);
}

void DeckMenu::edit()
{
    builder* edit = new builder(pages, deck);
    int index = pages->addWidget(edit);
    pages->setCurrentIndex(index);
}

void DeckMenu::stats()
{
    StatsView *sv = new StatsView(deck,pages);
    int index = pages->addWidget(sv);
    pages->setCurrentIndex(index);
}

void DeckMenu::back()
{
    int nextIndex = pages->currentIndex()-1;
    pages->removeWidget(pages->currentWidget());
    pages->setCurrentIndex(nextIndex);
}

void DeckMenu::saveDeckCallback()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Selected Deck"),
                                                    QDir::homePath() + QDir::separator() + "*",
                                                    "");

   deck->saveDeck(fileName);
}


