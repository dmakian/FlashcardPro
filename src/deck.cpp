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
    saveText += line;
    saveText += "\n";
    parseInfo(&line);

    line = instream.readLine();
    while(!line.isNull())
    {
        saveText += line;
        saveText += "\n";
        parseCard(&line);
        line = instream.readLine();
    }

}

Deck::Deck(std::vector<Flashcard *> _cards, QString deck_name_in) : cards(_cards)
{
    deck_name = deck_name_in;
    deck_score = 0;
    num_cards = _cards.size();

    makeSaveText();
}

Deck::~Deck()
{

}

Flashcard* Deck::getTop()
{
    return cards.at(0);
}

int Deck::getDeckScore()
{
    if(cards.size()==0) return 0;
    int min = cards[0]->getScore();
    for(int i=1;i<cards.size();i++){
        if(cards[i]->getScore() < min) min = cards[i]->getScore();
    }
    return min;
}

void Deck::shuffle()
{
    Flashcard* curCard = getTop();
    if(cards.size()>4){
        cards.erase(cards.begin());
        int i = 3;
        while(cards[i]->getScore() < curCard->getScore() && i<cards.size()-1) i++;
        cards.insert(cards.begin()+i,curCard);
    }
    else{
        cards.erase(cards.begin());
        cards.push_back(curCard);
    }
}

void Deck::parseInfo(QString* line)
{
    QString read;
    int i = 0;
    int j = 0;
    while(line->at(i) != char(30))
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    this->deck_name = read;

    read = "";
    ++i;
    j = 0;
    while(line->at(i) != char(30)) //&& line->at(i).isDigit())
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    if(line->at(i) != char(30))
        qDebug() << "Got a non digit value in deck size!";
    this->num_cards = read.toInt();

    read = "";
    ++i;
    j=0;
    while(i < line->size()) //&& line->at(i).isDigit())
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    this->deck_score = read.toInt();
}

void Deck::parseCard(QString* line)
{
    QString read;
    int i = 0;
    int j = 0;
    while(line->at(i) != char(30))
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    QString front = read;

    read = "";
    j = 0;
    if(line->at(i++) != char(30))
        qDebug() << "Wrong char at beginning of back of card!";

    while(line->at(i) != char(30))
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    QString back = read;

    read = "";
    j = 0;
    if(line->at(i++) != char(30))
        qDebug() << "Wrong char at beginning of card score!";

    while(i < line->size() && line->at(i).isDigit())
    {
        read[j] = line->at(i);
        ++i;
        ++j;
    }
    int score = read.toInt();

    Flashcard* new_card = new Flashcard(front, back, score);
    cards.push_back(new_card);
}

void Deck::makeSaveText()
{
    saveText += deck_name;
    saveText += char(30);
    saveText += num_cards;
    saveText += char(30);
    saveText += deck_score;
    saveText += '\n';

    for(int i = 0; i < num_cards; ++i)
    {
        saveText += cards[i]->fstr;
        saveText += char(30);
        saveText += cards[i]->bstr;
        saveText += char(30);
        saveText += cards[i]->cardScore;
        saveText += '\n';
    }

}

void Deck::saveDeck(QString fileName)
{
    if (fileName.isEmpty() || fileName.isNull())
      return;

    QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                qDebug() << "File couldn't be opened";
                return;
            }
    QTextStream out(&file);
    out << saveText;
    file.close();
}

Flashcard *Deck::getHardest()
{
    Flashcard* hardest;
    if(cards.size()==0) return hardest;
    hardest = cards[0];
    for(int i=1;i<cards.size();i++){
        if(cards[i]->getWrong() > hardest->getWrong()) hardest = cards[i];
    }
    return hardest;
}
