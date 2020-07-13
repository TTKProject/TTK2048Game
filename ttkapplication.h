#ifndef TTKAPPLICATION_H
#define TTKAPPLICATION_H

/* =================================================
 * This file is part of the TTK 2048 project
 * Copyright (C) 2017 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QWidget>

#define MAINWINDOW_HEIGHT       350
#define MAIMWINDOW_WIDTH		350
#define GRID_HEIGHT				75
#define GRID_WIDTH				75
#define VICTORY					2048
#define COORY					4
#define COORX					4

namespace Ui {
class TTKApplication;
}

class TTKApplication : public QWidget
{
    Q_OBJECT
public:
    enum CheckStatus
    {
        CS_WIN,
        CS_LOSE,
        CS_NULL
    };

    enum Direction
    {
        DR_UP,
        DR_DOWN,
        DR_LEFT,
        DR_RIGHT
    };

    explicit TTKApplication(QWidget *parent = 0);
    ~TTKApplication();

private Q_SLOTS:
    void startButtonClicked();

private:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    void initEachGrid();
    int getEmptyGridNumber();
    void randBlockNumber();
    QString getBlockColor(int number);
    CheckStatus checkBlockNumber();
    void calcBlocks(int dir);

    Ui::TTKApplication *m_ui;
    int m_block[COORX][COORY];
    bool m_canGetKeyToMove;
    int m_score;

};

#endif // TTKAPPLICATION_H
