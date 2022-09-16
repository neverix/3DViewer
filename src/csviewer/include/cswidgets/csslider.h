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

#ifndef _CS_CSSLIDER_H
#define _CS_CSSLIDER_H

#include "cswidgets/csparawidget.h"
#include <QSlider>

class CustomSlider : public QSlider
{
    Q_OBJECT
public:
    CustomSlider(Qt::Orientation orientation, QWidget* parent = nullptr) : QSlider(orientation, parent) {}
    void wheelEvent(QWheelEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
signals:
    void sliderClicked(int value);
};

class QLabel;
class CSLineEdit;
class QIntValidator;
class CSSlider : public CSParaWidget
{
    Q_OBJECT
public:
    CSSlider(int paraId, const char* title = "", QWidget* parent = nullptr);
    ~CSSlider();
    void setParaRange(const QVariant& min, const QVariant& max, const QVariant& step) override;
    void setValue(const QVariant& value) override;
    void retranslate(const char* context) override;

private slots:
    void onLineEditTextChanged();
    void onSliderValueChanged();
    void onSliderClicked(int value);
    void onLinEditFocusOut();
private:
    QLabel* titleLabel;
    CSLineEdit* lineEdit;
    CustomSlider* slider;
    QLabel* minLabel;
    QLabel* maxLabel;
    QIntValidator* intValidator;
    int step;
};
#endif // _CS_CSSLIDER_H