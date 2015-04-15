#include "login.h"
#include "newuser.h"
#include "userselect.h"
//#include <direct.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// LogIn Constructor //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LogIn::LogIn(QStackedWidget* pages_in)
    :QFrame(){
    pages = pages_in;
    layout = new QGridLayout();
    readCurrentUsers();
    initialSetup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Next Page Call /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogIn::nextPage()
{

    if(curUsers.empty()){
        qDebug() << "Add a setup page here because this means it's the first time ever using app";
        return;
    }

    int index = pages->addWidget(new UserSelect(pages, curUsers));
    pages->setCurrentIndex(index);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// Goes From Login to Create New Users Page ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogIn::createNewUserCallback(){

    int index = pages->addWidget(new NewUser(pages, this));
    pages->setCurrentIndex(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Sets Up Initial Login Page /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogIn::initialSetup(){

    QDir directory;
    fileLoc = directory.currentPath();
    int cutpoint = 0;
    for(int i = fileLoc.size() - 1; i > 0; i--){
        if(fileLoc.at(i) == '/'){
            cutpoint = i;
            i = 0;
        }
    }
    fileLoc = fileLoc.left(cutpoint);
    /*
    QString picLoc;
    picLoc = fileLoc;
    picLoc.append(":/flashcard.png");*/

    QPixmap newPixmap(":/flashcard.png");

    if (newPixmap.isNull()){
      QMessageBox msgBox;
      msgBox.critical(0,"Error", "Cannot open pixmap");
      msgBox.setFixedSize(500,200);
    }
    else{
        QLabel *newLabel = new QLabel();
        newLabel->setPixmap(newPixmap.scaled(500,500,Qt::KeepAspectRatio));
        newLabel->setMaximumSize(500,500);
        newLabel->setMinimumSize(500,500);
        layout->addWidget(newLabel,0,0);
    }

    QLabel *title_info = new QLabel("The most comprehensive flashcard app on the market. Two thumbs up!");
    QFont naxa;
    QFontDatabase db;
    naxa = db.font("Nexa Light","Normal",18);
    title_info->setFont(naxa);

    nextButton = new QPushButton("Log In");
    QPushButton *cr = new QPushButton("Sign Up");


    layout->addWidget(title_info,1,0);
    layout->addWidget(nextButton,2,0);
    layout->addWidget(cr,3,0);
    setLayout(layout);

    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
    connect(cr, SIGNAL(clicked()),this,SLOT(createNewUserCallback()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Goes from Create User to Login Page ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogIn::returnToLogInCallback(){

    for(int i=layout->count()-1; i >= 0; i--){
        QLayoutItem *item = layout->takeAt(i);
        layout->removeItem(item);
        item->widget()->hide();
    }
    initialSetup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Writes New User to Text File ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogIn::writeNewUserCallback(){
    QString txtFile;
    txtFile = fileLoc;
    txtFile.append("/FlashcardPro/login_names.txt");

    if(edit3->toPlainText() == ""){
        QMessageBox msgBox;
        msgBox.critical(0,"Error", "Please enter a username");
        msgBox.setFixedSize(500,200);
        return;
    }

    for(int i=0; i < currentUsers.size(); i++){
        if(edit3->toPlainText() == currentUsers.at(i)){
            QMessageBox msgBox;
            msgBox.critical(0,"Error", "Username Already exists. Please choose a new username");
            msgBox.setFixedSize(500,200);
            return;
        }
    }

    currentUsers.push_back(edit3->toPlainText());

    ofstream myFile;
    myFile.open(txtFile.toStdString().c_str(), std::ios::app);
    myFile << edit3->toPlainText().toStdString().c_str();
    myFile << "\n";
    myFile.close();

    returnToLogInCallback();

    QString folder;
    folder = fileLoc + "/" + edit3->toPlainText() + "_Decks";
    QDir dir(folder);
    //mkdir(folder.toStdString().c_str());
    if(!dir.exists()) dir.mkdir(".");
    else qDebug() << "Folder already exists!";


    //LOGIC FOR LOGGING INTO DATABASE HERE
    //int current = pages->currentIndex();
    pages->setCurrentIndex(1);

    QMessageBox msgBox;
    msgBox.setText("Successfully Created User!");
    msgBox.setFixedSize(500,200);
    msgBox.exec();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Reads in Users from Text File //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogIn::readCurrentUsers(){

    QString txtFile = fileLoc;
    txtFile.append("/FlashcardPro/login_names.txt");

    ifstream file(txtFile.toStdString().c_str());
    string temp;

    while(getline(file,temp)){
        QString push = QString::fromStdString(temp);
        if(push != "") currentUsers.push_back(push);

    }
}


void LogIn::userLoginCallback(){
    returnToLogInCallback();
    pages->setCurrentIndex(1);
}

void LogIn::addNewUser(User* user)
{
    curUsers.push_back(user);

    //write to login_names.txt
    QString txtFile = fileLoc;
    txtFile.append("/FlashcardPro/login_names.txt");
    ofstream myFile;
    myFile.open(txtFile.toStdString().c_str(), std::ios::app);
    myFile << user->username.toStdString().c_str();
    myFile << "\n";
    myFile.close();

    //Create directory to contain decks
    QString folder;
    folder = fileLoc + "/" + user->username + "_Decks";
    QDir dir(folder);
    //mkdir(folder.toStdString().c_str());
    if(!dir.exists()) dir.mkdir(".");
    else qDebug() << "Folder already exists!";

    //Go to choose user
    pages->removeWidget(pages->currentWidget());
    pages->setCurrentIndex(pages->currentIndex()-1);

    //Display success
    QMessageBox msgBox;
    msgBox.setText("Successfully Created User!");
    msgBox.setFixedSize(500,200);
    msgBox.exec();
}
