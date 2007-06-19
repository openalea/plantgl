/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
 *               
 *  ----------------------------------------------------------------------------
 * 
 *                      GNU General Public Licence
 *           
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */	
			
#include "translator.h"

#ifndef SYSTEM_IS__CYGWIN

#include <qtranslator.h>
#include <qapplication.h>
#include <plantgl/tool/util_enviro.h>

static QTranslator * fr = NULL;

void 
removeTranslator3(){
  if(fr)qApp->removeTranslator(fr);
}

void setFrenchTranslator3()
{
#ifdef __MINGW32__
# warning Translation message not set because of excessive computation time
#else
  if(!fr){
  fr = new QTranslator(NULL,"French");
  
  fr->insert(QTranslatorMessage("Viewer","&File","","&Fichier"));
  fr->insert(QTranslatorMessage("Viewer3","&Edit","","&Edition"));
  fr->insert(QTranslatorMessage("Viewer3","&View","","Afficha&ge"));
  fr->insert(QTranslatorMessage("Viewer3","&Tools","","Ou&tils"));
  fr->insert(QTranslatorMessage("Viewer3","&Help","","&Aide"));
  fr->insert(QTranslatorMessage("Viewer3","&Menu Bar","","Barre de &Menu"));
  fr->insert(QTranslatorMessage("Viewer3","&Control Panel","","Panneau de Contrôle"));
  fr->insert(QTranslatorMessage("Viewer3","&Tools Bar","","Barre d'ou&tils"));
  fr->insert(QTranslatorMessage("Viewer3","&Location Bar","","Barre d'adresse"));
  fr->insert(QTranslatorMessage("Viewer3","&Object Browser","","Explorateur d'&objets"));
  fr->insert(QTranslatorMessage("Viewer3","&Error Log","","Fenêtre d'&erreurs"));
  fr->insert(QTranslatorMessage("Viewer3","&Debug Log","","Fenêtre de &Debug"));
  fr->insert(QTranslatorMessage("Viewer3","GL Frame only","","Fenêtre GL seulement"));
  fr->insert(QTranslatorMessage("Viewer3","GL Frame Size","","Taille Fenêtre GL"));
  fr->insert(QTranslatorMessage("Viewer3","Customize","","Personnaliser"));
  fr->insert(QTranslatorMessage("Viewer3","Full Screen","","Plein Ecran"));
  fr->insert(QTranslatorMessage("Viewer3","PlantGL 3D Viewer","","Visualisateur 3D PlantGL"));
  fr->insert(QTranslatorMessage("Viewer3","Ready","","Prêt"));
  fr->insert(QTranslatorMessage("Viewer3","Exit","","Quitter"));
  fr->insert(QTranslatorMessage("Viewer3","Do you really want to exit ?","","Voulez vous vraiment quitter ?"));
  fr->insert(QTranslatorMessage("QMessageBox","Cancel","","Annuler"));
  fr->insert(QTranslatorMessage("QMessageBox","&Cancel","","Annuler"));
 
  fr->insert(QTranslatorMessage("ViewFileManager3","Open","","Ouvrir"));
  fr->insert(QTranslatorMessage("ViewFileManager3","&Open File","","&Ouvrir un fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Open File","","Ouvrir un fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Import","","Importer"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Export","","Exporter"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Save","","Enregistrer"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Save File","","Enregistrer le Fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","&Save","","Enregistrer"));
  fr->insert(QTranslatorMessage("ViewFileManager3","&Save As","","Enregistrer sous ..."));
  fr->insert(QTranslatorMessage("ViewFileManager3","ScreenShot","","Capture d'écran"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Save as Bitmap","","Enregistrer l'image"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Save Picture","","Enregistrer l'image"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Copy To Clipboard","","Copier dans le presse-papier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Copy Picture To Clipboard","","Copier l'image dans le presse-papier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Recents","","Fichiers récents"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Clear","","Vider l'historique"));
  fr->insert(QTranslatorMessage("ViewFileManager3","&Print...","","Im&primer..."));
  fr->insert(QTranslatorMessage("ViewFileManager3","&Refresh","","&Rafraichir"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Refresh","","Rafraichir"));
  fr->insert(QTranslatorMessage("ViewFileManager3","&Close","","&Fermer"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Properties","","Propriétés"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Save Configuration","","Sauver la Configuration"));  
  fr->insert(QTranslatorMessage("ViewFileManager3","Exit","","Quitter"));
  fr->insert(QTranslatorMessage("ViewFileManager3","File Exists","","Fichier existant"));
  fr->insert(QTranslatorMessage("ViewFileManager3"," already exists. Overwrite ?",""," existe déjà. Ecraser ?"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Yes","","Oui"));
  fr->insert(QTranslatorMessage("ViewFileManager3","No","","Non"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Files","","Fichiers"));
  fr->insert(QTranslatorMessage("ViewFileManager3","File","","Fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","All Files","","Tous les fichiers"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Open","","Ouvrir"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Save Image","","Enregistrer une Image"));
  fr->insert(QTranslatorMessage("ViewFileManager3","File Name Error","","Erreur de nom de fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager3","File name of index %1 doesn't exist !","","Le nom de fichier d'index %1 n'existe pas!"));
  // fr->insert(QTranslatorMessage("ViewFileManager3","Login","","Login"));
  // fr->insert(QTranslatorMessage("ViewFileManager3","Login :","","Login :"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Password","","Mot de passe"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Password :","","Mot de passe :"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Cannot open temporary file","","Impossible d'ouvrir le fichier temporaire"));
  fr->insert(QTranslatorMessage("ViewFileManager3","File Download Failed","","Echec du telechargement"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Network Error : ","","Problème réseau : "));
  fr->insert(QTranslatorMessage("ViewFileManager3","Temporary File","","Le fichier temporaire"));
  fr->insert(QTranslatorMessage("ViewFileManager3","does not exist.","","n'existe pas."));
  fr->insert(QTranslatorMessage("ViewFileManager3","Transfert progress","","progression du transfert"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Incompatible init file version","","La version du fichier d'initialisation est incompatible"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Incompatible init file version","","La version du fichier d'initialisation est incompatible"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Version","","Version"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Current Version","","Version Courante"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Init file","","Fichier d'initialisation"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Cannot access to init file","","Fichier d'initialisation inaccessible"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Cannot access to existing init file","","Fichier d'initialisation existant inaccessible"));
  
  fr->insert(QTranslatorMessage("ViewLocationBar3"," Location ",""," Adresse "));
  fr->insert(QTranslatorMessage("ViewLocationBar3","Erase Location","","Efface l'Adresse"));
  fr->insert(QTranslatorMessage("ViewLocationBar3","The Filename","","Le Nom de fichier courant"));

  fr->insert(QTranslatorMessage("ViewHelpMenu3","What's &This?","","Qu'est ce que c'est?"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","&Help","","Aide"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","&About Viewer","","&A propos du Viewer"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","&License","","&Licence"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","About &Qt","","A propos de &Qt"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","About Qt","","A propos de Qt"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Technical Characteristics","","Caractéristiques techniques"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Technical Characteristics","","Caractéristiques techniques"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Qt Hierarchy","","Hiérarchie Qt"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Style","","Style"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","How to use Viewer","","Utilisation du Visualisateur"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","PlantGL Viewer","","Visualisateur PlantGL"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Version","","Version"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Geom Library","","Bibliotheque GEOM"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Binary Format Version","","Version du Format Binaire"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Real Type Precision","","Précision du type Real"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Double","","Double"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Simple","","Simple"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Using Threads","","Utilisation des Threads"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Geom Namespace","","Espace de Nom GEOM"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","True","","Vrai"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","False","","Faux"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Geom Debug","","Geom Debug"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Geom DLL","","Geom DLL"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Using Glut","","Utilisation de Glut"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Tools Library","","Bibliothèque Tools"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Tools Namespace","","Espace de Nom Tools"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Using RogueWave","","Utilisation de RogueWave"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Install Path","","Repertoire d'installation"));
  fr->insert(QTranslatorMessage("ViewHelpMenu3","Symbol Path","","Repertoire des Symboles"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Flex","","Flex"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu3","Amapmod","","Amapmod"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","&Save","","&Enregistrer"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","&Cancel","","&Annuler"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","&Ok","","&Ok"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Information","","Information"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Values","","Valeurs"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Machine","","Machine"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Processor","","Processeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Number of processor","","Nombre de Processeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Number of processor","","Nombre de Processeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","System","","Système"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Language","","Langue"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Version","","Version"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","True","","Vrai"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","False","","Faux"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Yes","","Oui"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","No","","Non"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Disable","","Désactivé"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Word Size","","Taille de mot"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Byte Order","","Ordre des bits"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Big Endian","","Big Endian"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Little Endian","","Little Endian"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Process","","Processus"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Id","","Id"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Compilation","","Compilation"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Date","","Date"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Compiled on","","Compilé sur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Process","","Processus"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","at","","à"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Build Mode","","Mode de compilation"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","C++ Compiler","","Compilation C++"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Run-Time Type Information","","Informations sur les types à l'execution"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Exception Handling","","Gestion des Exceptions"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Compilation Optimization","","Optimisation à la compilation"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","C++ Standard's Version","","Version du Standard C++"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Qt Library","","Bibliothèque Qt"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Thread Support","","Support des Threads"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Qt DLL","","Qt DLL"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Default Font","","Fonts par défaut"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Charset","","Charset"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Family","","Famille"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Size","","Taille"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","GL Widget","","Fenêtre GL"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","GL Context","","Contexte GL"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","GL Format","","Format GL"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Default","","Par Defaut"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Valid","","Valide"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Shared","","Partagé"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Direct Rendering","","Rendu Direct"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Double Buffer","","Tampon Double"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Depth Buffer","","Tampon de Profondeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Alpha channel","","Couleur Alpha"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Accumulation buffer","","Tampon d'Accumulation"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Stencil buffer","","Tampon de tracé"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Stereo buffering","","Stockage en Stéréo"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Overlay Plane","","Plan de Recouvrement"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Plane","","Plan"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Overlay GL Context","","Contexte GL de Recouvrement"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Overlay GL Format","","Format GL de Recouvrement"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","None","","Aucun"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Vendor","","Vendeur"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Client","","Client"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Renderer","","Renderer"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Extension(s)","","Extension(s)"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","OpenGL Utility Library (GLU)","","Bibliothèque OpenGL Utility (GLU)"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","OpenGL Utility Toolkit Library (Glut)","","Bibliothèque OpenGL Utility Toolkit (Glut)"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","OpenGL Windows Extension (WGL)","","Extension OpenGL de Windows (WGL)"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","OpenGL X Extension (GLX)","","Extension OpenGL de X (GLX)"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Screen","","Ecran"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Screens","","Ecrans"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Number of Screen","","Nombre d'Ecran"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Server","","Serveur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Display Name","","Nom de l'affichage"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Revision","","Révision"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Resolution","","Résolution"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Default Depth","","Profondeur par défaut"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Number of Entries in Default Colormap","","Taille de la carte de couleur par défaut"));
//  fr->insert(QTranslatorMessage("ViewSysInfo3","Backing Store","","Backing Store"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","When Mapped","","En cas de mapping"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Not Useful","","Pas Utile"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Always","","Toujours"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Save Unders","","Save Unders"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Supported","","Supporté"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Not Supported","","Non Supporté"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Connection Number","","Numero de Connection"));
  // fr->insert(QTranslatorMessage("ViewSysInfo3","Format","","Format"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Pixmap Format","","Format de Pixmap"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Depth","","Profondeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo3","Bits per pixel","","Bites par pixel"));

  fr->insert(QTranslatorMessage("ViewGLFrame3","Set Background Color to","","Couleur de Fond à"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","GL Error","","Erreur GL"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Abort","","Abandonner"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Continue","","Continuer"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Set Line Width to","","Epaisseur de Ligne assignée à"));
  fr->insert(QTranslatorMessage("ViewGLFrame3"," Line Width ",""," Epaisseur Ligne "));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Line Width","","Epaisseur Ligne"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Mode Multiple Selection","","Mode Sélection Multiple"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Mode Selection","","Mode Sélection"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Selection cleared","","Sélection effacée"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Clear Selection","","Effacer la Sélection"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Selection","","Sélection"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Rectangle Selection","","Sélection Rectangulaire"));
  // fr->insert(QTranslatorMessage("ViewGLFrame3","Rectangle","","Rectangule"));
  // fr->insert(QTranslatorMessage("ViewGLFrame3","Point","","Point"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Mouse on","","Pointeur en"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Selection from","","Selection de"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","to","","à"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Save screenshot with format","","Enregistre la capture d'écran au format"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","in","","sous"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Copy screenshot to clipboard","","Copie la capture d'écran dans le presse papier"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Cannot access global clipboard","","Impossible d'acceder au presse papier global"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","System Error","","Erreur Système"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Edit Line Width","","Editer l'épaisseur de ligne"));
  // fr->insert(QTranslatorMessage("ViewGLFrame3","Renderer","","Renderer"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Camera","","Caméra"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Light","","Lumière"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Fog","","Brouillard"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Grid","","Grille"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Clipping Plane","","Plan de Coupe"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Rotating Center","","Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Background Color","","Couleur de fond"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","GL Options","","Options GL"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Culling","","Elimination des faces"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","None","","Aucunes"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Back Face","","Faces Arrières"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Front Face","","Faces Avants"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Shade Model","","Modèle d'Ombrage"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Flat","","Plat"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Smooth","","Souple"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Color/Material Dithering","","Couleur/Matériel Dithering"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Hidden Surface Removal","","Elimination des surfaces cachées"));
  fr->insert(QTranslatorMessage("ViewGLFrame3","Normals Normalization","","Normalisation des Normales"));

  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Open &Geom File","","Ouvrir un Fichier &Geom"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","&Add Geom File","","&Ajouter un Fichier Geom"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Open &2 Geom File","","Ouvrir 2 Fichiers Geom"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open &Geom File","","Ouvrir un Fichier &Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","&Add Geom File","","&Ajouter un Fichier Geom"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Save As &Geom","","Enregistrer au Format &Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Save &Selection","","Enregistrer la Sélection"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Save &Not Selection","","Enregistrer Tout sauf la Sélection"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Import &AmapSymbol","","Importer un Symbole &Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Import &Linetree Files","","Importer une &Ligne Elastique"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Import &GeomView Files","","Importer un Fichier &GeomView"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Import &VegeStar Files","","Importer un Fichier &VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Import VegeStar Symbol","","Importer les Symboles &VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Import Symbol","","Importer le Symbole"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Show Symbol","","Afficher le Symbole"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Set Selection as Symbol","","Assigner la Sélection au Symbole"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Export as &AmapSymbol","","Exporter en Symbole &Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Export as &Linetree","","Exporter en &Ligne Elastique"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Export as Pov&Ray","","Exporter en Pov&Ray"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Export as Vr&ml","","Exporter en Vr&ml"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Export as Ply","","Exporter en Ply"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Export as V&egeStar","","Exporter en V&egeStar"));

  fr->insert(QTranslatorMessage("ViewImporterSelection","File Format not recognized !","","Format de Fichier non reconnu!"));
  fr->insert(QTranslatorMessage("ViewImporterSelection","File :","","Fichier:"));
//  fr->insert(QTranslatorMessage("ViewImporterSelection","Type :","","Type :"));
  fr->insert(QTranslatorMessage("ViewImporterSelection","Choose Importer :","","Importer avec :"));
  fr->insert(QTranslatorMessage("ViewImporterSelection","&Cancel","","&Annuler"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Filename","","Nom de Fichier Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Cannot open Empty filename","","Impossible d'ouvrir un Nom de Fichier Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Already Reading File","","Lecture de Fichier en cours"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Currently Reading File","","En cours de Lecture du Fichier"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Already Reading File","","Lecture de Fichier en cours"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Currently Reading File","","En cours de Lecture du Fichier"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open GEOM File","","Ouvrir Fichier GEOM"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Add GEOM File","","Ajouter Fichier GEOM"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Geom File","","Fichier Geom"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Open GEOM File","","Ouvrir Fichier GEOM"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Geom File","","Fichier Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Amap Symbol","","Symbole Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open Amap Symbol","","Ouvrir Symbole Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","VegeStar File","","Fichier VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open VegeStar File","","Ouvrir Fichier VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open VegeStar File","","Ouvrir Fichier VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","VegeStar Symbol","","Symbole VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open VegeStar Symbol","","Ouvrir Symbole VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Open VegeStar Symbol","","Ouvrir Symbole VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Selection","","Sélection Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","No Shape are selected!","","Aucune Forme Sélectionnée!"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Separated File for","","Différents Fichiers pour"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Geometry","","Géométrie"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Appearance","","Apparence"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Save","","Enregistrer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","File Already Exists","","Fichier existant"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Shape File","","Fichier Forme"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Geometry File","","Fichier Géométrie"));
  fr->insert(QTranslatorMessage("ViewFileManager3","Appearance File","","Fichier Appearance"));
  fr->insert(QTranslatorMessage("ViewFileManager3"," already exists. Overwrite ?",""," existe déjà. Ecraser ?"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Yes","","Oui"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Yes To All","","Toujours Oui"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Cancel","","Annuler"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","File Generated with PlantGL 3D Viewer","","Fichier généré par PlantGL 3D Viewer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","PovRay File","","Fichier PovRay"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Cfg File","","Fichier Cfg"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Cannot write file","","Impossible d'écrire dans le fichier"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Vrml File","","Fichier Vrml"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Ply Format","","Format Ply"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Ply File","","Fichier Ply"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Binary Little Endian","","Binaire Little Endian"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Binary Big Endian","","Binaire Big Endian"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","GEOM Error","","Erreur GEOM"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Scene","","Scene Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Scene to Add","","Scene à Ajouter Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Scene Not Valid","","Scene Invalide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","To continue can cause dysfunction of this program","","Continuer peut causer un dysfonctionnement de ce programme"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","File","","Fichier"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Line","","Ligne"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Abort","","Annuler"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Continue","","Continuer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Validity","","Validité"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Display","","Afficher"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","geometric shapes.","","formes géométriques"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Display empty scene.","","Afficher Scene Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Shape","","Forme"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","unselected","","desélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","selected","","sélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","and","","et"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","shape selected","","forme sélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","shapes selected","","formes sélectionnées"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","shape unselected","","forme desélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","shapes unselected","","formes desélectionnées"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","not found","","pas trouvée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","not found","","pas trouvée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Selection. Cannot Remove!","","Sélection vide. Impossible de Supprimer!"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Confirmation","","Confirmation"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Remove Selection?","","Supprimer la Sélection?"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Selection. Cannot Replace!","","Sélection vide. Impossible de Remplacer!"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Remove Selection","","Supprimer la Sélection"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Keep Selection Only","","Garder Seulement la Sélection"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Replace Selection by","","Remplacer la Sélection par"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Wire","","Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Discretization","","Discretisation"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","General Properties","","Propriétés Générales"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Number of Element","","Nombre d'Elément"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Number of Element","","Nombre d'Elément"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","shape(s)","","forme(s)"));
//  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Surface","","Surface"));
//  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Volume","","Volume"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Number of Polygon","","Nombre de Polygone"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Memory Size","","Taille Mémoire"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Memory Size","","Taille Mémoire"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Bounding Box","","Boîte Englobante"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Upper Rigth Corner","","Sommet Haut Droit"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Lower Left Corner","","Sommet Bas Gauche"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Lower Left Corner","","Sommet Bas Gauche"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Size","","Taille"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Center","","Centre"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Empty Scene","","Scene Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Geom &Scene","","&Scene Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Per Vertex","","Par Sommet"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Per Face","","Par Face"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","&Normal","","&Normale"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL3","Recompute","","Recalculer"));
  // fr->insert(QTranslatorMessage("ViewGeomSceneGL3","&Display List","","&Display List"));
  // fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Transition","","Transition"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL3","Transition Slider","","Slider de Transition"));
  fr->insert(QTranslatorMessage("ViewEditMatDialog3","&Apply","","&Appliquer"));
  fr->insert(QTranslatorMessage("ViewEditMatDialog3","&Reset","","&Réinitialiser"));
//  fr->insert(QTranslatorMessage("ViewEditMatDialog3","&Ok","","&Ok"));
  fr->insert(QTranslatorMessage("ViewEditMatDialog3","&Cancel","","A&nnuler"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Edit Material","","Editer le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Dissociate Material","","Dissocier le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Copy Material","","Copier le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Paste Material","","Coller le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Material edition can be apply only on selected shapes.","","L'Edition de Matériel s'applique seulement sur les formes sélectionnées."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Material edition can be apply on one material.","","L'Edition de Matériel s'applique seulement sur un Matériel."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","At least one shape must be selected to dissociate material.","","Au moins une forme doit être sélectionnée pour dissocier le matériel."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","The Material of the first shape will be copy on all selected shapes.","","Le matériel de la première forme va être copié sur toutes les formes sélectionnées."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","One shape must be selected to copy material.","","Une forme doit être sélectionnée pour copier le matériel."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","Cannot copy multiple Material.","","Impossible de copier plusieurs matériels."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","GEOM Error","","Erreur GEOM"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","No material available.","","Aucun matériel disponible."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL3","At least one shape must be selected to paste material.","","Au moins une forme doit être sélectionnée pour coller le matériel."));
  fr->insert(QTranslatorMessage("ViewMultiscaleEditGeomSceneGL3","GEOM Error","","Erreur GEOM"));
  fr->insert(QTranslatorMessage("ViewMultiscaleEditGeomSceneGL3","Fit Geometry","","Ajuster une forme"));
  fr->insert(QTranslatorMessage("ViewMultiscaleEditGeomSceneGL3","No Geometry to Fit.","","Aucune Géométrie à Ajuster."));
  fr->insert(QTranslatorMessage("ViewApproximationForm","Input Geometry","","Géométrie en entrée"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","&Input Geometry","","Géométrie en ent&rée"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","Output Geometry","","Géométrie en sortie"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","&Whole Scene","","Toute la &scene"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","&Selection","","&Sélection"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","&All except Selection","","&Tout sauf Sélection"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","Algorithm","","Algorithme"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","Material","","Matériel"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","&Edit","","&Editer"));
//  fr->insert(QTranslatorMessage("ViewApproximationForm","&Approximation","","&Approximation"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","Geometry &not approximated","","Géométrie &non approximée"));
//  fr->insert(QTranslatorMessage("ViewApproximationForm","&Ok","","&Ok"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","&Cancel","","Ann&uler"));
  fr->insert(QTranslatorMessage("ViewApproximationForm","Error during Fit computation.","","Erreur durant le calcul d'ajustement."));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Volume Rendering","","Rendu Volumique"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Volume and Wire Rendering","","Rendu Volumique et Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Wire Rendering","","Rendu Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Skeleton Rendering","","Rendu du Squelette"));
  // fr->insert(QTranslatorMessage("ViewModalRendererGL3","Volume","","Volume"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Volume and Wire","","Volume et Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Wire","","Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Skeleton","","Squelette"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Control Points","","Points de Contrôle"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Control Points Rendering","","Rendu des Points de Contrôle"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Control Points Rendering Enable","","Rendu des Points de Contrôle Activé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Control Points Rendering Disable","","Rendu des Points de Contrôle Désactivé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Bounding Box","","Boîtes Englobantes"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Bounding Box Rendering","","Rendu des Boîtes Englobantes"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Bounding Box Rendering Enable","","Rendu des Boîtes Englobantes Activé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Bounding Box Rendering Disable","","Rendu des Boîtes Englobantes Désactivé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Light Rendering","","Rendu avec Lumière"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL3","Light","","Lumière"));
//  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","&Volume","","&Volume"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","&Wire","","&Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","S&keleton","","S&quelete"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","Volu&me and Wire","","Volu&me et Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","&Control Points","","Points de &Contrôle"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","&Bounding Box","","&Boîtes Englobantes"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu3","&Light","","&Lumière"));
//  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Home","","&Home"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Front View (YZ)","","Vue de &Face (YZ)"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Right View (XZ)","","Vue de &Droite (XZ)"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Top View (XY)","","Vue de De&ssus (XY)"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","GEOM System","","Système GEOM"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","GL System","","Système GL"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Change","","&Changer"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","Coordinates System","","Système de Coordonnées"));
  // fr->insert(QTranslatorMessage("ViewCameraMenu3","&Perspective","","&Perspective"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","Ort&hographic","","Ort&hographique"));
  // fr->insert(QTranslatorMessage("ViewCameraMenu3","Projection","","Projection"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Save","","Enregi&strer"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Read","","Cha&rger"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Auto Fit to Window","","Auto-Ajustement à la Fenêtre"));
  fr->insert(QTranslatorMessage("ViewCameraMenu3","&Fit to Window","","Ajuster à la Fenêtre"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Perspective Camera","","Caméra en Perspective"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Orthographic Camera","","Caméra Orthographique"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","GEOM Coordinates System","","Système de Coordonnées GEOM"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","GL Coordinates System","","Système de Coordonnées GL"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","&Camera","","&Caméra"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Home Position","","Position Home"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Camera Position","","Position Camera"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Front View (YZ)","","Vue de &Face (YZ)"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Right View (XZ)","","Vue de &Droite (XZ)"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Top View (XY)","","Vue de De&ssus (XY)"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Fit to Window","","Ajuster à la Fenêtre"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","File Access","","Accès Fichier"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Cannot Open File","","Impossible d'Ouvrir le Fichier"));
  fr->insert(QTranslatorMessage("ViewCameraGL3","Abort","","Abandon"));
  // fr->insert(QTranslatorMessage("CameraProperties","Zoom","","Zoom"));
  // fr->insert(QTranslatorMessage("CameraProperties","Translation","","Translation"));
  // fr->insert(QTranslatorMessage("CameraProperties","Azimuth","","Azimuth"));
  fr->insert(QTranslatorMessage("CameraProperties","Elevation","","Elévation"));
  fr->insert(QTranslatorMessage("CameraProperties","Near Plane","","Plan Proche"));
  fr->insert(QTranslatorMessage("CameraProperties","Far Plane","","Plan Eloigné"));
  fr->insert(QTranslatorMessage("CameraProperties","Eye","","Oeil"));
  fr->insert(QTranslatorMessage("CameraProperties","Center","","Centre"));
  fr->insert(QTranslatorMessage("CameraProperties","Projection","","Projection"));
  fr->insert(QTranslatorMessage("CameraProperties","Perspective","","Perspective"));
  fr->insert(QTranslatorMessage("CameraProperties","Orthographic","","Orthographique"));
  fr->insert(QTranslatorMessage("CameraProperties","Default View Angle","","Angle de Vue par Défaut"));
  fr->insert(QTranslatorMessage("CameraProperties","Current View Angle","","Angle de Vue courant"));
  fr->insert(QTranslatorMessage("CameraProperties","Coordinates System","","Système de Coordonnées"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Clipping Planes","","Plan de Coupe"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Plane 1","","Plan 1"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Plane 2","","Plan 2"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Plane 3","","Plan 3"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Plane 4","","Plan 4"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Plane 5","","Plan 5"));
  fr->insert(QTranslatorMessage("ClippingPlaneWidget","Plane 6","","Plan 6"));
  fr->insert(QTranslatorMessage("ViewClippingPlaneGL3","Clipping Planes Control","","Contrôle des Plans de Coupe"));
  fr->insert(QTranslatorMessage("ViewCPlaneMenu3","Control","","Contrôle"));
  fr->insert(QTranslatorMessage("ViewCPlaneMenu3","Plane","","Plan"));
  // fr->insert(QTranslatorMessage("ClippingPlaneWidget","&Ok","","&Ok"));
  // fr->insert(QTranslatorMessage("ViewLightMenu3","&Home","","&Home"));
  fr->insert(QTranslatorMessage("ViewLightMenu3","on X axis","","sur l'Axe X"));
  fr->insert(QTranslatorMessage("ViewLightMenu3","on Y axis","","sur l'Axe Y"));
  fr->insert(QTranslatorMessage("ViewLightMenu3","on Z axis","","sur l'Axe Z"));
  fr->insert(QTranslatorMessage("ViewLightMenu3","Visible","","Visible"));
  fr->insert(QTranslatorMessage("ViewLightGL3","Light Source Visible","","Source Lumineuse Visible"));
  fr->insert(QTranslatorMessage("ViewLightGL3","Light Source Invisible","","Source Lumineuse Invisible"));
  fr->insert(QTranslatorMessage("ViewFogGL3","Fog Control","","Contrôle du Brouillard"));
  fr->insert(QTranslatorMessage("FogWidget","Fog","","Brouillard"));
  fr->insert(QTranslatorMessage("FogWidget","Mode","","Mode"));
  fr->insert(QTranslatorMessage("FogWidget","Start","","Début"));
  fr->insert(QTranslatorMessage("FogWidget","End","","Fin"));
  fr->insert(QTranslatorMessage("FogWidget","Density","","Densité"));
  fr->insert(QTranslatorMessage("FogWidget","Color","","Couleur"));
  // fr->insert(QTranslatorMessage("FogWidget","Hints","","Hints"));
  fr->insert(QTranslatorMessage("ViewFogGL3","Control","","Contrôle"));
  fr->insert(QTranslatorMessage("ViewFogGL3","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ViewGridMenu3","XY Plane","","Plan XY"));
  fr->insert(QTranslatorMessage("ViewGridMenu3","XZ Plane","","Plan XZ"));
  fr->insert(QTranslatorMessage("ViewGridMenu3","YZ Plane","","Plan YZ"));
  fr->insert(QTranslatorMessage("ViewGridMenu3","Axis","","Axes"));
  fr->insert(QTranslatorMessage("ViewGridGL3","XY Plane","","Plan XY"));
  fr->insert(QTranslatorMessage("ViewGridGL3","XZ Plane","","Plan XZ"));
  fr->insert(QTranslatorMessage("ViewGridGL3","YZ Plane","","Plan YZ"));
  fr->insert(QTranslatorMessage("ViewGridGL3","Axis","","Axes"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL3","Rotating Center","","Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL3","Visible Rotating Center","","Centre de Rotation Visible"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL3","Enable Rotating Center",""," Activer le Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL3","Center Rotating Center","","Centrer le Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL3","Center Rotating Center","","Centrer le Centre de Rotation"));
//  fr->insert(QTranslatorMessage("ViewRotCenterMenu3","&Home","","&Home"));
//  fr->insert(QTranslatorMessage("ViewRotCenterMenu3","&Visible","","&Visible"));
  fr->insert(QTranslatorMessage("ViewRotCenterMenu3","&Center","","&Centrer"));
  fr->insert(QTranslatorMessage("ViewRotCenterMenu3","&Enable","","&Activer"));
  fr->insert(QTranslatorMessage("ViewRotCenterMenu3","&Control","","&Contrôle"));
  fr->insert(QTranslatorMessage("ViewProperties3","Properties","","Propriétés"));
  fr->insert(QTranslatorMessage("ViewProperties3","Name","","Nom"));
//  fr->insert(QTranslatorMessage("ViewProperties3","Location","","Location"));
  fr->insert(QTranslatorMessage("ViewProperties3","Size","","Taille"));
  fr->insert(QTranslatorMessage("ViewProperties3","Owner","","Propriétaire"));
  fr->insert(QTranslatorMessage("ViewProperties3","Last Modified","","Dernière Modif."));
  fr->insert(QTranslatorMessage("ViewProperties3","Last Accessed","","Dernièr Accès"));
  fr->insert(QTranslatorMessage("ViewProperties3","&File","","&Fichier"));
  // fr->insert(QTranslatorMessage("ViewProperties3","C&onfig","","C&onfig"));
  fr->insert(QTranslatorMessage("ViewProperties3","Saved Options","","Options Sauvegardées"));
  fr->insert(QTranslatorMessage("ViewProperties3","Window Position and Size","","Position et Taille de Fenêtre"));
  fr->insert(QTranslatorMessage("ViewProperties3","Window Style","","Style de Fenêtre"));
  fr->insert(QTranslatorMessage("ViewProperties3","ToolBars States (Experimental)","","Etats des Barres d'Outils (Experimental)"));
  fr->insert(QTranslatorMessage("ViewProperties3","File History","","Historique des Fichiers"));
  fr->insert(QTranslatorMessage("ViewProperties3","BackGround Color","","Couleur de Fond"));
  fr->insert(QTranslatorMessage("ViewProperties3","Grids Visibility","","Visibilité des Grilles"));
  fr->insert(QTranslatorMessage("ViewProperties3","Camera/Grid Automatic Fitting","","Ajustement Automatique des Caméra/Grilles"));
  fr->insert(QTranslatorMessage("ViewProperties3","SpinBox instead of Dials in Control Panel","","SpinBox à la place des Dials"));
  fr->insert(QTranslatorMessage("ViewProperties3","Appearance Options","","Options d'Apparence"));
  fr->insert(QTranslatorMessage("ViewProperties3","use SpinBox instead of Dials in Control Panel","","Utilisation de SpinBox à la place des Dial"));
  fr->insert(QTranslatorMessage("ViewProperties3","show Initialization Dialog at Startup","","Montrer la fenêtre d'Initialisation au démarage"));
  fr->insert(QTranslatorMessage("ViewProperties3","Language","","Langue"));
  fr->insert(QTranslatorMessage("ViewProperties3","Note: Language change takes effect only at next startup.","","Note: Modifier la langue ne prendra effet qu'au prochain démarrage"));

  fr->insert(QTranslatorMessage("ViewBrowser3","Browser","","Explorateur"));
  fr->insert(QTranslatorMessage("QBrowser","Browser :","","Explorateur :"));
  fr->insert(QTranslatorMessage("QBrowser","Full &Mode","","&Mode Etendu"));
  fr->insert(QTranslatorMessage("QBrowser","&Cancel","","&Annuler"));
  // fr->insert(QTranslatorMessage("QBrowser","&Ok","","&Ok"));
  fr->insert(QTranslatorMessage("ViewErrorDialog3","Viewer Error Dialog","","Fenêtre d'Erreur du Viewer"));
  fr->insert(QTranslatorMessage("MessageDisplayer","Verbose","","Bavard"));
  fr->insert(QTranslatorMessage("MessageDisplayer","Popup when Errors","","Apparaitre en cas d'erreurs"));
  fr->insert(QTranslatorMessage("MessageDisplayer","Cl&ear","","&Effacer"));
  fr->insert(QTranslatorMessage("MessageDisplayer","&Cancel","","&Annuler"));
  // fr->insert(QTranslatorMessage("MessageDisplayer","&Ok","","&Ok"));
  // fr->insert(QTranslatorMessage("MessageDisplayer","Message :","","Message :"));

  fr->insert(QTranslatorMessage("ViewControlPanel3","Camera","","Caméra"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Elevation","","Elévation"));
  // fr->insert(QTranslatorMessage("ViewControlPanel3","Azimuth","","Azimuth"));
  // fr->insert(QTranslatorMessage("ViewControlPanel3","Zoom","","Zoom"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Move","","Pas"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Light Position","","Position Lumière"));
//  fr->insert(QTranslatorMessage("ViewControlPanel3","Distance","","Distance"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Light Material","","Matériel Lumière"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Ambient","","Ambiant"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Diffuse","","Diffus"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Specular","","Spéculaire"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Grids","","Grilles"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Unit","","Unité"));
  fr->insert(QTranslatorMessage("ViewControlPanel3","Size","","Taille"));

  fr->insert(QTranslatorMessage("QColorDialog","Cancel","","Annuler"));
  fr->insert(QTranslatorMessage("QColorDialog","&Add To Custom Colors","","Ajouter au Couleurs Personnalisées"));
  fr->insert(QTranslatorMessage("QColorDialog","&Custom colors","","&Couleurs Personnalisées"));
  fr->insert(QTranslatorMessage("QColorDialog","Select color","","Selection de Couleur"));
  fr->insert(QTranslatorMessage("QColorDialog","&Basic colors","","Couleurs de &Base"));
  fr->insert(QTranslatorMessage("QColorDialog","&Define Custom Colors >>","","Définit comme Couleur Personnalisée >>"));
  fr->insert(QTranslatorMessage("QColorDialog","&Red:","","&Rouge:"));
  fr->insert(QTranslatorMessage("QColorDialog","&Green:","","&Vert:"));
  fr->insert(QTranslatorMessage("QColorDialog","Bl&ue:","","Ble&u:"));
  
  // fr->save(TOOLS(getPlantGLDir())+"/share/plantgl/lang/french.qm");
  }

  qApp->installTranslator( fr );
 // qWarning("French Translator installed");
#endif
}

QStringList 
getAvailableLanguage3(){
  QStringList l("English");
  l.append("French");
  return l;
}
#endif
/* ----------------------------------------------------------------------- */
