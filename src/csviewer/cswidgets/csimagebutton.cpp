/*******************************************************************************
* This file is part of the 3DViewer                                            *
*                                                                              *
* Copyright (C) 2022 Revopoint3D Company Ltd.                                  *
* All rights reserved.                                                         *
*                                                                              *
* This program is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation, either version 3 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* This program is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)             *
* for more details.                                                            *
*                                                                              *
********************************************************************************/

#include "csimagebutton.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>

CSTextImageButton::CSTextImageButton(const QIcon& icon, const char* text, Qt::LayoutDirection layoutDirection, QWidget* parent)
    : QPushButton(parent)
    , layoutDirection(layoutDirection)
    , icon(icon)
    , buttonTitle(text)
{
    setCheckable(true);
    initButton();

    buttonText->setProperty("ButtonTitle", true);
}

void CSTextImageButton::retranslate(const char* context)
{
    if (strlen(buttonTitle) > 0)
    {
        buttonText->setText(QApplication::translate(context, buttonTitle));
    }
}

void CSTextImageButton::initButton()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    buttonText = new QLabel(tr(buttonTitle), this);
    buttonIcon = new QLabel(this);

    auto size = icon.availableSizes().first();
    buttonIcon->setFixedSize(size);
    buttonIcon->setPixmap(icon.pixmap(size));

    if (layoutDirection == Qt::LeftToRight)
    {
        layout->addWidget(buttonText);
        layout->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
        layout->addWidget(buttonIcon);
    }
    else 
    {
        layout->addWidget(buttonIcon);
        layout->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
        layout->addWidget(buttonText);
    }

    connect(this, &QPushButton::toggled, [=](bool checked) 
        {
            if (checked)
            {
                buttonIcon->setPixmap(icon.pixmap(size, QIcon::Selected, QIcon::On));
            }
            else 
            {
                buttonIcon->setPixmap(icon.pixmap(size));
            }
        });
}
