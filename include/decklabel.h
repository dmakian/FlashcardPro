#ifndef DECKLABEL_H
#define DECKLABEL_H

#include <QtWidgets>
#include "deck.h"

class DeckLabel:public QLabel
{
    Q_OBJECT
public:
    explicit DeckLabel(Deck* deck_in, QStackedWidget* pages_in, QGridLayout* grid_in, QWidget * parent = 0 );
    ~DeckLabel();
    void updateName();
    void SELFDESTRUCT();

private:
    Deck* deck;
    QStackedWidget* pages;
    QGridLayout* grid;
    bool hover;

signals:
    void clicked();

protected:
    void mousePressEvent ( QMouseEvent * event );
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);

public slots:
    void saveDeckCallback();
    void openDeck();

};

#endif // DECKLABEL_H

