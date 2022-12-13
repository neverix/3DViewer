/*******************************************************************************
* This file is part of the 3DViewer
*
* Copyright 2022-2026 (C) Revopoint3D AS
* All rights reserved.
*
* Revopoint3D Software License, v1.0
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistribution of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistribution in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of Revopoint3D AS nor the names of its contributors may be used
* to endorse or promote products derived from this software without specific
* prior written permission.
*
* 4. This software, with or without modification, must not be used with any
* other 3D camera than from Revopoint3D AS.
*
* 5. Any software provided in binary form under this license must not be
* reverse engineered, decompiled, modified and/or disassembled.
*
* THIS SOFTWARE IS PROVIDED BY REVOPOINT3D AS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL REVOPOINT3D AS OR CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Info:  https://www.revopoint3d.com
******************************************************************************/

#include "cswidgets/cscombobox.h"

#include <QListView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

CSComboBox::CSComboBox(int paraId, const char* title, QWidget* parent, const char* tips)
    : CSParaWidget(paraId, title, parent)
    , comboBox(new CustomComboBox(this))
    , titleLabel(new QLabel(this))
    , lastIndex(-1)
    , tips(tips)
{
    //setAttribute(Qt::WA_StyledBackground);
    setObjectName("CSComboBox");
    titleLabel->setObjectName("TitleLabel");

    QHBoxLayout* layout = new QHBoxLayout(this);

    titleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(titleLabel);
    layout->addWidget(comboBox);

    // add tips button
    if (strlen(tips) > 0)
    {
        tipsButton = new QPushButton(this);
        tipsButton->setObjectName("tipsButton");
        layout->addWidget(tipsButton);
    }

    layout->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QListView* listView = new QListView(this);
    comboBox->setFocusPolicy(Qt::NoFocus);
    listView->setFocusPolicy(Qt::NoFocus);

    comboBox->setView(listView);

    bool suc = true;
    suc &= (bool)connect(comboBox, SIGNAL(activated(int)), this, SLOT(onComboBoxIndexChanged(int)));

    Q_ASSERT(suc);
}

CSComboBox::~CSComboBox()
{

}

void CSComboBox::setItems(const QList<QPair<QString, QVariant>>& items)
{
    comboBox->clear();
    for (const auto& item : items)
    {
        comboBox->addItem(item.first, item.second);
    }
}

void CSComboBox::setValue(const QVariant& value)
{
    const int curIdx = comboBox->currentIndex();
    lastIndex = curIdx;

    for (int i = 0; i < comboBox->count(); i++)
    {
        auto itemData = comboBox->itemData(i);
        if (itemData == value)
        {
            if (i != curIdx)
            {
                comboBox->setCurrentIndex(i);
                lastIndex = comboBox->currentIndex();
            }
            break;
        }
    }
}

void CSComboBox::getValue(QVariant& value)
{
    value = comboBox->itemData(comboBox->currentIndex());
}

void CSComboBox::onComboBoxIndexChanged(int index)
{
    if (lastIndex != index)
    {
        lastIndex = index;
        emit valueChanged(getParaId(), comboBox->itemData(index));
    }
}

void CSComboBox::retranslate(const char* context)
{
    if (strlen(paraName) > 0)
    {       
        titleLabel->setText(QApplication::translate(context, paraName));
    }

    if(tipsButton)
    {
        tipsButton->setToolTip(QApplication::translate(context, tips));
    }
}

void CSComboBox::clearValues()
{
    comboBox->clear();
}
