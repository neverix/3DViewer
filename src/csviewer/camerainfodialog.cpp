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

#include "camerainfodialog.h"
#include "./ui_camerainfo.h"

CameraInfoDialog::CameraInfoDialog(QWidget* parent)
    : QDialog(parent)
    , cameraInfoUi(new Ui::CameraInfoWidget)
{
    cameraInfoUi->setupUi(this);
    setWindowFlags(this->windowFlags() & Qt::WindowCloseButtonHint);
    setModal(true);
}

CameraInfoDialog::~CameraInfoDialog()
{
    delete cameraInfoUi;
}

void CameraInfoDialog::updateCameraInfo(const CSCameraInfo& info)
{
    cameraInfoUi->modelLabel->setText(info.model);
    cameraInfoUi->snLabel->setText(info.cameraInfo.serial);
    cameraInfoUi->sdkVerLabel->setText(info.sdkVersion);
    cameraInfoUi->firmwareVerLabel->setText(info.cameraInfo.firmwareVersion);
    cameraInfoUi->algoVerLabel->setText(info.cameraInfo.algorithmVersion);

    CSConnectType connectMode = (CSConnectType)info.connectType;
    if (connectMode == CONNECT_TYPE_NET)
    {
        cameraInfoUi->connectionMode->setText("Network");
    }
    else
    {
        cameraInfoUi->connectionMode->setText("USB");
    }
}

void CameraInfoDialog::onTranslate()
{
    cameraInfoUi->retranslateUi(this);
}