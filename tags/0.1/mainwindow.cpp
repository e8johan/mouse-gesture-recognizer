/*
 * This file is part of the mouse gesture package.
 * Copyright (C) 2006 Johan Thelin <e8johan@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the
 * following conditions are met:
 *
 *   - Redistributions of source code must retain the above
 *     copyright notice, this list of conditions and the
 *     following disclaimer.
 *   - Redistributions in binary form must reproduce the
 *     above copyright notice, this list of conditions and
 *     the following disclaimer in the documentation and/or
 *     other materials provided with the distribution.
 *   - The names of its contributors may be used to endorse
 *     or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "mainwindow.h"

#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>

MainWindow::MainWindow() : QMainWindow()
{
    QWidget *base = new QWidget( this );

    cb1 = new QCheckBox( "No 1", base );
    cb2 = new QCheckBox( "No 2", base );
    cb3 = new QCheckBox( "No 3", base );
    cb4 = new QCheckBox( "No 4", base );
    cb5 = new QCheckBox( "No 5", base );

    QWidget *buttonBase = new QWidget( base );

    QPushButton *check = new QPushButton( "Check", buttonBase );
    QPushButton *clear = new QPushButton( "Clear", buttonBase );

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget( check );
    hlayout->addWidget( clear );
    buttonBase->setLayout( hlayout );

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget( cb1 );
    vlayout->addWidget( cb2 );
    vlayout->addWidget( cb3 );
    vlayout->addWidget( cb4 );
    vlayout->addWidget( cb5 );
    vlayout->addWidget( buttonBase );
    base->setLayout( vlayout );

    connect( check, SIGNAL(clicked()), SLOT(setAll()) );
    connect( clear, SIGNAL(clicked()), SLOT(clearAll()) );

    setCentralWidget( base );
}

void MainWindow::clearAll()
{
    cb1->setChecked( false );
    cb2->setChecked( false );
    cb3->setChecked( false );
    cb4->setChecked( false );
    cb5->setChecked( false );
}

void MainWindow::setAll()
{
    cb1->setChecked( true );
    cb2->setChecked( true );
    cb3->setChecked( true );
    cb4->setChecked( true );
    cb5->setChecked( true );
}

void MainWindow::noMatch()
{
    QMessageBox::warning( this, "Error",
        "Failed to recognize the performed mouse gesture<br>"
        "<br>"
        "<b>Supported gestures</><br>"
        "- Sideways * 3 - clear all<br>"
        "- Up-Left - set all<br>" );
}

void MainWindow::secretMessage()
{
    QMessageBox::information( this, "Congratulation",
        "<b>Congratulations!</b><br>"
        "You found the secret message!" );
}
