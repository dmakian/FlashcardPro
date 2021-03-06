//Study Area
#include "studyarea.h"
#include "flashcard.h"
#include <QtWidgets>
#include <QtGui>

StudyArea::StudyArea(Deck *_deck, QStackedWidget* pages_in, DeckMenu* parent_in)
{
    scores = new QLabel;
    grid = new QGridLayout;
    parent = parent_in;
    deck = _deck;
    pages = pages_in;
    curCard = _deck->getTop();
    curCard->setToolTip("Click to flip!");
    grid->addWidget(curCard,1,0,1,3);

    QPushButton *back = new QPushButton("Back");
    back->setShortcut(QKeySequence(" "));
    connect(back, SIGNAL(clicked()), this, SLOT(goBack()));
    grid->addWidget(back,0,0,1,1);

    QPushButton *right = new QPushButton;
    right->setToolTip("Click if you knew the answer");
    right->setFixedSize(65,65);
    QPixmap pm(":/green_check.png");
    pm = pm.scaled(65, 65, Qt::KeepAspectRatio);
    QIcon ic(pm);
    right->setIcon(ic);
    QSize iconSize(65, 65);
    right->setIconSize(iconSize);
    right->setShortcut(QKeySequence(Qt::Key_Right));
    grid->addWidget(right,2,2,1,1,Qt::AlignLeft);
    connect(right,SIGNAL(clicked()),this,SLOT(correct()));

    QPushButton *wrong = new QPushButton;
    wrong->setToolTip("Click if you didn't know the answer.");
    wrong->setFixedSize(65,65);
    QPixmap pm2(":/red_x.png");
    pm2 = pm2.scaled(65, 65, Qt::KeepAspectRatio);
    QIcon ic2(pm2);
    wrong->setIcon(ic2);
    wrong->setIconSize(iconSize);
    wrong->setShortcut(QKeySequence(Qt::Key_Left));
    grid->addWidget(wrong,2,0,1,1,Qt::AlignRight);
    connect(wrong,SIGNAL(clicked()),this,SLOT(incorrect()));

    QFont smallfont;
    QFontDatabase db;

    smallfont = db.font("Nexa Light", "Normal", 18);
    back->setFont(smallfont);

    scores->setText("Card Score: " + QString::number(curCard->getScore()));
    grid->addWidget(scores,2,1,1,1,Qt::AlignCenter);

    grid->setRowStretch(0,0);
    grid->setRowStretch(1,1);
    grid->setRowStretch(2,0);
    this->setLayout(grid);
}

StudyArea::~StudyArea()
{

}

void StudyArea::correct()
{
    curCard->correct();
    deck->shuffle();
    deck->getDeckScore();
    grid->removeWidget(curCard);
    curCard->hide();
    if (!curCard->isFront()) curCard->Flip();
    curCard = deck->getTop();
    grid->addWidget(curCard,1,0,1,3);
    curCard->show();
    scores->setText("Card Score: " + QString::number(curCard->getScore()));
    deck->autoSave();

}

void StudyArea::incorrect()
{
    curCard->incorrect();
    deck->shuffle();
    grid->removeWidget(curCard);
    curCard->hide();
    if (!curCard->isFront()) curCard->Flip();
    curCard = deck->getTop();
    grid->addWidget(curCard,1,0,1,3);
    curCard->show();
    scores->setText("Card Score: " + QString::number(curCard->getScore()));
    deck->autoSave();
}

void StudyArea::goBack()
{
    if(pages == NULL) return;

    DeckMenu* newMen = new DeckMenu(deck, pages);

    pages->removeWidget(pages->currentWidget());
    pages->removeWidget(pages->currentWidget());
    int index = pages->addWidget(newMen);
    pages->setCurrentIndex(index);

}

