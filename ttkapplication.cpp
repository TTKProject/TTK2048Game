#include "ttkapplication.h"
#include "ui_ttkapplication.h"

#include <QPainter>
#include <QDateTime>
#include <QKeyEvent>

TTKApplication::TTKApplication(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::TTKApplication)
{
    m_ui->setupUi(this);
    setFixedSize(540, 375);

    m_canGetKeyToMove = false;
    m_score = 0;
    initEachGrid();

    connect(m_ui->startButton, SIGNAL(clicked()), SLOT(startButtonClicked()));
}

TTKApplication::~TTKApplication()
{
    delete m_ui;
}

void TTKApplication::startButtonClicked()
{
    m_canGetKeyToMove = true;
    m_score = 0;
    initEachGrid();

    randBlockNumber();
    randBlockNumber();

    m_ui->stateLabel->clear();
    m_ui->scoreLabel->setText("0");

    update();
}

void TTKApplication::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(QRect(10, 10, MAIMWINDOW_WIDTH, MAINWINDOW_HEIGHT), QColor(187, 173, 160));
    for(int row=0; row<COORX; row++)
    {
        for(int col=0; col<COORY; col++)
        {
            painter.fillRect( 20 + ( 10 + GRID_HEIGHT ) * row, 20 + ( 10 + GRID_WIDTH ) * col,
                              GRID_HEIGHT, GRID_WIDTH, QColor(205, 193, 179));
        }
    }

    for(int row=0; row<COORX; row++)
    {
        for(int col=0; col<COORY; col++)
        {
            if(m_block[row][col] != 0)
            {
                painter.drawPixmap(20 + ( 10 + GRID_HEIGHT ) * row, 20 + ( 10 + GRID_WIDTH ) * col,
                                    GRID_HEIGHT, GRID_WIDTH, QPixmap(getBlockColor(m_block[row][col])));
            }
            else
            {
                painter.fillRect(20 + ( 10 + GRID_HEIGHT ) * row, 20 + ( 10 + GRID_WIDTH ) * col,
                                 GRID_HEIGHT, GRID_WIDTH, QColor(205, 193, 179));
            }
        }
    }

}

void TTKApplication::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if(!m_canGetKeyToMove)
    {
        return;
    }

    switch(event->key())
    {
        case 87: calcBlocks(DR_UP); break;
        case 83: calcBlocks(DR_DOWN); break;
        case 65: calcBlocks(DR_LEFT); break;
        case 68: calcBlocks(DR_RIGHT); break;
        default: break;
    }

}

void TTKApplication::initEachGrid()
{
    for(int row=0; row<COORX; row++)
    {
        for(int col=0; col<COORY; col++)
        {
            m_block[row][col] = 0;
        }
    }
}

int TTKApplication::getEmptyGridNumber()
{
    int count = 0;
    for(int row=0; row<COORX; row++)
    {
        for(int col=0; col<COORY; col++)
        {
            if(m_block[row][col] == 0)
            {
                ++count;
            }
        }
    }
    return count;
}

void TTKApplication::randBlockNumber()
{
    int count=0, randnum;
    qsrand( QDateTime::currentMSecsSinceEpoch() );

    randnum = qrand() % getEmptyGridNumber();

    for(int row=0; row<COORX; row++)
    {
        for(int col=0; col<COORY; col++)
        {
            if(m_block[row][col] == 0 && count++ == randnum)
            {
                m_block[row][col] = (qrand()%2 + 1) * 2;
                return;
            }
        }
    }
}

QString TTKApplication::getBlockColor(int number)
{
    switch( number )
    {
        case 2:
        case 4:
        case 8:
        case 16:
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
        case 1024:
        case 2048:
            return ":/data/" + QString::number(number);
        default:
            return ":/data/4096";
    }
}

TTKApplication::CheckStatus TTKApplication::checkBlockNumber()
{
    for(int row=0; row<COORX; row++)
    {
        for(int col=0; col<COORY; col++)
        {
            if(m_block[row][col] >= VICTORY)
            {
                return CS_WIN;
            }

            if(m_block[row][col] == 0)
            {
                return CS_NULL;
            }

            if((col < 3) && (m_block[row][col] == m_block[row][col + 1] || m_block[col][row] == m_block[col + 1][row]))
            {
                return CS_NULL;
            }
        }
    }
    return CS_LOSE;
}

void TTKApplication::calcBlocks(int dir)
{
    int f0=0, fb=0, fm=0, dx=0, dy=0, nx=0, ny=0;
    for(int j=0; j<COORX; j++)
    {
        for(int k=0; k<COORY-1; k++)
        {
            if(fb == 1)
            {
                fb = 0;
                break;
            }

            for(int i=0; i<COORY-1; i++)
            {
                switch(dir)
                {
                    case DR_LEFT:
                        {
                            dx = i;
                            dy = j;
                            nx = 1;
                            ny = 0;
                            break;
                        }
                    case DR_DOWN:
                        {
                            dx = j;
                            dy = 3 - i;
                            nx = 0;
                            ny = -1;
                            break;
                        }
                    case DR_RIGHT:
                        {
                            dx = 3 - i;
                            dy = j;
                            nx = -1;
                            ny = 0;
                            break;
                        }
                    case DR_UP:
                        {
                            dx = j;
                            dy = i;
                            nx = 0;
                            ny = 1;
                            break;
                        }
                    default: break;
                }

                if(m_block[dx][dy] == 0)
                {
                    if(m_block[dx+nx][dy+ny] !=0)
                    {
                        f0 = 1;
                    }
                    m_block[dx][dy] = m_block[dx + nx][dy + ny];
                    m_block[dx + nx][dy + ny] = 0;
                }
                else if(m_block[dx][dy] != 0 && m_block[dx][dy] == m_block[dx + nx][dy + ny])
                {
                    m_block[dx][dy] = m_block[dx][dy] + m_block[dx + nx][dy + ny];
                    m_block[dx + nx][dy + ny] = 0;
                    m_score += m_block[dx][dy];
                    fb = 1;
                    fm = 1;
                }
            }
        }
    }

    if(getEmptyGridNumber() > 0 &&( f0 == 1 || fm == 1 ))
    {
        randBlockNumber();
    }

    if(checkBlockNumber() == CS_WIN)
    {
        m_ui->stateLabel->setText("You Win!");
        m_canGetKeyToMove = false;
    }
    else if(checkBlockNumber() == CS_LOSE)
    {
        m_ui->stateLabel->setText("You Lose!");
        m_canGetKeyToMove = false;
    }
    m_ui->scoreLabel->setText(QString::number(m_score));

    update();
}
