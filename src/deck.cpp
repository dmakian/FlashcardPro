#include "deck.h"
#include "flashcard.h"
#include <QTextStream>
#include <QDebug>

Deck::Deck(FILE* deck_file)
{
    QTextStream instream(deck_file, QIODevice::ReadOnly);

    QString line;
    line = instream.readLine();
    if(line.isNull())
    {
        qDebug() << "Error reading in deck file!";
        exit(1);
    }
    parseInfo(&line);

    line = instream.readLine();
    while(!line.isNull())
    {
        parseCard(&line);
        line = instream.readLine();
    }

}

Deck::~Deck()
{

}
void Deck::parseInfo(QString* line)
{
    QString read;
    int i = 0;
    int j = 0;
    while(line->at(i) != '@')
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    this->deck_name = read;

    read = "";
    ++i;
    j = 0;
    while(line->at(i) != '@' && line->at(i).isDigit())
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    if(line->at(i) != '@')
        qDebug() << "Got a non digit value in deck size!";
    this->num_cards = read.toInt();

    read = "";
    ++i;
    j=0;
    while(i < line->size() && line->at(i).isDigit())
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    if(i < line->size())
        qDebug() << "Got a non digit value in deck score!";
    this->deck_score = read.toInt();
}

void Deck::parseCard(QString* line)
{
    QString read;
    int i = 0;
    int j = 0;
    if(line->at(i++) != '<')
        qDebug() << "Wrong char at beginning of card line!";
    while(line->at(i) != '>')
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    QString front = read;

    read = "";
    j = 0;
    if(line->at(++i) != '<')
        qDebug() << "Wrong char at beginning of back of card!";
    ++i;
    while(line->at(i) != '>')
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    QString back = read;

    read = "";
    j = 0;
    if(line->at(++i) != '<')
        qDebug() << "Wrong char at beginning of card score!";
    ++i;
    while(line->at(i) != '>' && line->at(i).isDigit())
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    if(line->at(i) != '>')
        qDebug() << "Non digit in card score!";
    int score = read.toInt();

    Flashcard* new_card = new Flashcard(front, back, score);
    cards.push_back(new_card);
}

void Deck::saveDeck(QString location)
{

}


