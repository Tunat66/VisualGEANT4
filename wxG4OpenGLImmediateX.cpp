#include "wxG4OpenGLImmediateX.h"
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
// 
// Andrew Walkden  27th March 1996, modified by Tuna Tasali, Sep. 2022
// OpenGL graphics system factory.


wxG4OpenGLImmediateX::wxG4OpenGLImmediateX() :
    G4VGraphicsSystem("wxOpenGLImmediateX",
        "wxOGLIX",
        G4VisFeaturesOfOpenGLIX()/*no need to change*/,
        G4VGraphicsSystem::threeD)
{
    G4OpenGLViewerMessenger::GetInstance();
}

wxG4OpenGLImmediateX::~wxG4OpenGLImmediateX() {}

G4VSceneHandler* wxG4OpenGLImmediateX::CreateSceneHandler(const G4String& name) {
    G4VSceneHandler* pScene = new G4OpenGLImmediateSceneHandler(*this, name);
    return    pScene;
}

G4VViewer* wxG4OpenGLImmediateX::CreateViewer(G4VSceneHandler& scene,
    const G4String& name) {
    G4VViewer* pView =
        new G4OpenGLImmediateXViewer((G4OpenGLImmediateSceneHandler&)scene, name);
    if (pView) {
        if (pView->GetViewId() < 0) {
            G4cerr << "wxG4OpenGLImmediateX::CreateViewer: error flagged by negative"
                " view id in wxG4OpenGLImmediateXViewer creation."
                "\n Destroying view and returning null pointer."
                << G4endl;
            delete pView;
            pView = 0;
        }
    }
    else {
        G4cerr << "wxG4OpenGLImmediateX::CreateViewer: null pointer on"
            " new wxG4OpenGLImmediateXViewer." << G4endl;
    }
    return pView;
}