/**
 * @file form.cpp
 * @brief Hlavičový soubor pro hrací desku.
 *
 * Modul slouží k práci s hrací deskou.
 *
 * @author Jan Pavlica
 */

#include "form.h"
#include "ui_form.h"

#include <iostream>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ai = false;
    // nastavení rámečků a pozadí
    setStyleSheet("\
              #frame{\
                  background-image: url(:/back_tabs.jpg);\
                  border-width:2px;\
                  border-color: black;\
                  border-style:solid ; \
              }  \
              #frame_1{\
                  background-image: url(:/back_tabs.jpg);\
                  border-width:2px;\
                  border-color: black;\
                  border-style:solid ; \
              }  \
              #frame_2{\
                  background-image: url(:/back_tabs.jpg);\
                  border-width:2px;\
                  border-color: black;\
                  border-style:solid ; \
              }  \
              #frame_3{\
                  background-image: url(:/back_tabs.jpg);\
                  border-width:2px;\
                  border-color: black;\
                  border-style:solid ; \
              }  \
              #board{\
                  background-image: url(:/back_tabs.jpg);\
                  border-width:2px;\
                  border-color: black;\
                  border-style:solid ; \
              }  \
                           ");
}

Form::~Form()
{
    delete ui;
    delete myLayout;
    delete plus;
    for (int i=0; i<cellsList.length(); i++)
        delete cellsList.at(i);
    cellsList.clear();

    for (int i=0; i<desList.length(); i++)
        delete desList.at(i);
    desList.clear();
    //delete gameInstance;
}

// nastaveni pozice pro políčko

void Cell::setPosition(int pos)
{
    position = pos;
}
// zjištění pozice políčka
void Cell::getPosition()
{

    int x = position % pomSize;
    int y = position / pomSize;
    // položení kamene
    othRet ret = gameInstance->putToken(x,y);
    //kontrola odpovědi
    switch(ret)
    {
        case othRet::VALID_TURN:
        myBoard->ui->Msg->setText("Valid move.");
            break;
    case othRet::INVALID_TURN:
        myBoard->ui->Msg->setText("Invalid move.");
        break;
    case othRet::GAME_OVER:
        myBoard->ui->Msg->setText("Game over.");
        QMessageBox::information(
            this,
            tr("Game Over"),
            tr("No more avaliable moves.") );
        break;
    case othRet::INTERNAL_ERROR:
        myBoard->ui->Msg->setText("Internal error.");
        break;
    case othRet::REDO_LIMIT:
        myBoard->ui->Msg->setText("You cannot go forward.");
        break;
    case othRet::UNDO_LIMIT:
        myBoard->ui->Msg->setText("You cannot go back.");
        break;
    case othRet::REPEATE_TURN:
        myBoard->ui->Msg->setText("Your opponent cannot move now. Your turn.");
        break;
    default:
        break;
    }
}

// propojení akce s políčkem
Cell::Cell(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(getPosition()));
}

Cell::~Cell(){}
// nastavení velikosti hrací desky
void Form::setDeskSize(int size)
{
    this->size = size;
}
// nastavení desky
void Form::setUpDesk(int size)
{
    int completeSize = size+1;
    int pomPosition = 0;
    myLayout = new QGridLayout();
    myLayout->setVerticalSpacing(0);
    myLayout->setHorizontalSpacing(0);
    myLayout->setMargin(1);
    
    //inicializace desky
    for(int i = 0; i <(completeSize*completeSize); i++)
    {
        if(i < completeSize && i != 0)
        {
            //číselné popisovače
            QLabel *descrip = new QLabel();
            descrip->setText(QString::number(i));
            descrip->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            descrip->setAlignment(Qt::AlignCenter);
            descrip->setStyleSheet("\
                                   QLabel{\
                                       border-width:1px;\
                                       border-color: black;\
                                       border-style:solid ; \
                                   }  \
                                   ");
                                   myLayout->addWidget(descrip,i/completeSize,i%completeSize);
                    desList.push_back(descrip);
        }
        else if(i%completeSize == 0 && i != 0)
        {
            //popisovače abecedou
            QLabel *descrip = new QLabel();
            descrip->setText(QString(QChar(64+(i/completeSize))));
            descrip->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            descrip->setAlignment(Qt::AlignCenter);
            descrip->setStyleSheet("\
                                   QLabel{\
                                       border-width:1px;\
                                       border-color: black;\
                                       border-style:solid ; \
                                   }  \
                                   ");
                                   myLayout->addWidget(descrip,i/completeSize,i%completeSize);
                    desList.push_back(descrip);
        }
        else if(i != 0) /*      image:url(:/bila.svg) 0 0 0 0 stretch stretch;\*/
        {
            // inicializace políček
            Cell *cell = new Cell();
            cell->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            cell->setFlat(true);
            cell->setFocusPolicy(Qt::FocusPolicy::NoFocus);
            cell->setPosition(pomPosition);
            pomPosition++;
            cell->pomSize = size;
            cell->myBoard = this;
            cellsList.push_back(cell);
            cell->setStyleSheet("\
                                QPushButton{\
                                    border-width:1px;\
                                    border-color: black;\
                                    border-style:solid ; \
                                }  \
                                ");
                                myLayout->addWidget(cell,i/completeSize,i%completeSize);
        }
        else
        {
            // políčko navíc pro zachování velikosti čtverce
            Cell *plonk = new Cell();
            plonk->setFlat(true);
            plonk->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            plonk->setFocusPolicy(Qt::FocusPolicy::NoFocus);
            plonk->setStyleSheet("\
                                 QPushButton{\
                                     border-width:1px;\
                                     border-color: black;\
                                     border-style:solid ; \
                                 }  \
                                 ");
                                 myLayout->addWidget(plonk,0,0);
                    this->plus = plonk;
        }
    }
    ui->board->setLayout(myLayout);

}

// vykreslovací funkce
void Form::reDrawDesk()
{
    bool turn = gs->get_turn();
    const int* score = gs->get_score();
    othBoardPtr b = gs->get_board();

    if(turn && ai)
    {
        delay(1500);
    }
    // player 1
    ui->Score1Type->setText(QString::number(*score));
    ui->StonesLeft1Type->setText(QString::number(((size*size))-(*score) ));
    score++;
    //player 2
    ui->Score2Type->setText(QString::number(*score));
    ui->StoneLeft2Type->setText(QString::number(((size*size))-(*score) ));

    // obarvení kamene pro tag
    if(turn)
    {
        QPixmap pixmap(":/cerna.svg");
        ui->TurnIcon->setPixmap(pixmap);
    }
    else
    {
        QPixmap pixmap(":/bila.svg");
        ui->TurnIcon->setPixmap(pixmap);
    }
    // přebarvení všech políček
    for(int i = 0; i < (size*size); i++){
        if(b[0][i] == BitStateType::BLACK)
            cellsList.at(i)->setStyleSheet("\
                                           QPushButton{\
                                               image:url(:/cerna.svg) 0 0 0 0 stretch stretch;\
                                               border-width:1px;\
                                               border-color: black;\
                                               border-style:solid ; \
                                           }  \
                                           ");
                                           else if(b[0][i] == BitStateType::WHITE)
                    cellsList.at(i)->setStyleSheet("\
                                                   QPushButton{\
                                                       image:url(:/bila.svg) 0 0 0 0 stretch stretch;\
                                                       border-width:1px;\
                                                       border-color: black;\
                                                       border-style:solid ; \
                                                   }  \
                                                   ");
                                                   else if(b[0][i] == BitStateType::GREY)
                    cellsList.at(i)->setStyleSheet("\
                                                   QPushButton{\
                                                       image:url(:/seda.svg) 0 0 0 0 stretch stretch;\
                                                       border-width:1px;\
                                                       border-color: black;\
                                                       border-style:solid ; \
                                                   }  \
                                                   ");
                                                   else if(b[0][i] == BitStateType::EMPTY)
                    cellsList.at(i)->setStyleSheet("\
                                                   QPushButton{\
                                                       border-width:1px;\
                                                       border-color: black;\
                                                       border-style:solid ; \
                                                   }  \
                                                   ");

    }


}

void Form::setGameInstanceForCells()
{
    // nastavení aktuální herní instance všem políčkám
    for (int i =0; i < (size*size); i++)
    {
        cellsList.at(i)->gameInstance = this->gameInstance;
    }
}

void Form::delay( int millisecondsToWait )
{
    // zpoždění pro AI kvůli přirozenosti tahů
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
