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
			
#include "view_translator.h"

#ifndef SYSTEM_IS__CYGWIN

#include <qtranslator.h>
#include <qapplication.h>

static QTranslator * fr = NULL;

void 
removeTranslator(){
  if(fr)qApp->removeTranslator(fr);
}

void setFrenchTranslator()
{
  if(!fr){
  fr = new QTranslator(NULL,"French");
  
  fr->insert(QTranslatorMessage("Viewer","&File","","&Fichier"));
  fr->insert(QTranslatorMessage("Viewer","&Edit","","&Edition"));
  fr->insert(QTranslatorMessage("Viewer","&View","","Afficha&ge"));
  fr->insert(QTranslatorMessage("Viewer","&Tools","","Ou&tils"));
  fr->insert(QTranslatorMessage("Viewer","&Help","","&Aide"));
  fr->insert(QTranslatorMessage("Viewer","&Menu Bar","","Barre de &Menu"));
  fr->insert(QTranslatorMessage("Viewer","&Control Panel","","Panneau de Contrôle"));
  fr->insert(QTranslatorMessage("Viewer","&Tools Bar","","Barre d'ou&tils"));
  fr->insert(QTranslatorMessage("Viewer","&Location Bar","","Barre d'adresse"));
  fr->insert(QTranslatorMessage("Viewer","&Object Browser","","Explorateur d'&objets"));
  fr->insert(QTranslatorMessage("Viewer","&Error Log","","Fenêtre d'&erreurs"));
  fr->insert(QTranslatorMessage("Viewer","&Debug Log","","Fenêtre de &Debug"));
  fr->insert(QTranslatorMessage("Viewer","GL Frame only","","Fenêtre GL seulement"));
  fr->insert(QTranslatorMessage("Viewer","GL Frame Size","","Taille Fenêtre GL"));
  fr->insert(QTranslatorMessage("Viewer","Customize","","Personnaliser"));
  fr->insert(QTranslatorMessage("Viewer","Full Screen","","Plein Ecran"));
  fr->insert(QTranslatorMessage("Viewer","PlantGL 3D Viewer","","Visualisateur 3D PlantGL"));
  fr->insert(QTranslatorMessage("Viewer","Ready","","Prêt"));
  fr->insert(QTranslatorMessage("Viewer","Exit","","Quitter"));
  fr->insert(QTranslatorMessage("Viewer","Do you really want to exit ?","","Voulez vous vraiment quitter ?"));
  fr->insert(QTranslatorMessage("QMessageBox","Cancel","","Annuler"));
  fr->insert(QTranslatorMessage("QMessageBox","&Cancel","","Annuler"));
 
  fr->insert(QTranslatorMessage("ViewFileManager","Open","","Ouvrir"));
  fr->insert(QTranslatorMessage("ViewFileManager","&Open File","","&Ouvrir un fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager","Open File","","Ouvrir un fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager","Import","","Importer"));
  fr->insert(QTranslatorMessage("ViewFileManager","Export","","Exporter"));
  fr->insert(QTranslatorMessage("ViewFileManager","Save","","Enregistrer"));
  fr->insert(QTranslatorMessage("ViewFileManager","Save File","","Enregistrer le Fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager","&Save","","Enregistrer"));
  fr->insert(QTranslatorMessage("ViewFileManager","&Save As","","Enregistrer sous ..."));
  fr->insert(QTranslatorMessage("ViewFileManager","ScreenShot","","Capture d'écran"));
  fr->insert(QTranslatorMessage("ViewFileManager","Save as Bitmap","","Enregistrer l'image"));
  fr->insert(QTranslatorMessage("ViewFileManager","Save Picture","","Enregistrer l'image"));
  fr->insert(QTranslatorMessage("ViewFileManager","Copy To Clipboard","","Copier dans le presse-papier"));
  fr->insert(QTranslatorMessage("ViewFileManager","Copy Picture To Clipboard","","Copier l'image dans le presse-papier"));
  fr->insert(QTranslatorMessage("ViewFileManager","Recents","","Fichiers récents"));
  fr->insert(QTranslatorMessage("ViewFileManager","Clear","","Vider l'historique"));
  fr->insert(QTranslatorMessage("ViewFileManager","&Print...","","Im&primer..."));
  fr->insert(QTranslatorMessage("ViewFileManager","&Refresh","","&Rafraichir"));
  fr->insert(QTranslatorMessage("ViewFileManager","Refresh","","Rafraichir"));
  fr->insert(QTranslatorMessage("ViewFileManager","&Close","","&Fermer"));
  fr->insert(QTranslatorMessage("ViewFileManager","Properties","","Propriétés"));
  fr->insert(QTranslatorMessage("ViewFileManager","Save Configuration","","Sauver la Configuration"));  
  fr->insert(QTranslatorMessage("ViewFileManager","Exit","","Quitter"));
  fr->insert(QTranslatorMessage("ViewFileManager","File Exists","","Fichier existant"));
  fr->insert(QTranslatorMessage("ViewFileManager"," already exists. Overwrite ?",""," existe déjà. Ecraser ?"));
  fr->insert(QTranslatorMessage("ViewFileManager","Yes","","Oui"));
  fr->insert(QTranslatorMessage("ViewFileManager","No","","Non"));
  fr->insert(QTranslatorMessage("ViewFileManager","Files","","Fichiers"));
  fr->insert(QTranslatorMessage("ViewFileManager","File","","Fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager","All Files","","Tous les fichiers"));
  fr->insert(QTranslatorMessage("ViewFileManager","Open","","Ouvrir"));
  fr->insert(QTranslatorMessage("ViewFileManager","Save Image","","Enregistrer une Image"));
  fr->insert(QTranslatorMessage("ViewFileManager","File Name Error","","Erreur de nom de fichier"));
  fr->insert(QTranslatorMessage("ViewFileManager","File name of index %1 doesn't exist !","","Le nom de fichier d'index %1 n'existe pas!"));
  // fr->insert(QTranslatorMessage("ViewFileManager","Login","","Login"));
  // fr->insert(QTranslatorMessage("ViewFileManager","Login :","","Login :"));
  fr->insert(QTranslatorMessage("ViewFileManager","Password","","Mot de passe"));
  fr->insert(QTranslatorMessage("ViewFileManager","Password :","","Mot de passe :"));
  fr->insert(QTranslatorMessage("ViewFileManager","Cannot open temporary file","","Impossible d'ouvrir le fichier temporaire"));
  fr->insert(QTranslatorMessage("ViewFileManager","File Download Failed","","Echec du telechargement"));
  fr->insert(QTranslatorMessage("ViewFileManager","Network Error : ","","Problème réseau : "));
  fr->insert(QTranslatorMessage("ViewFileManager","Temporary File","","Le fichier temporaire"));
  fr->insert(QTranslatorMessage("ViewFileManager","does not exist.","","n'existe pas."));
  fr->insert(QTranslatorMessage("ViewFileManager","Transfert progress","","progression du transfert"));
  fr->insert(QTranslatorMessage("ViewFileManager","Incompatible init file version","","La version du fichier d'initialisation est incompatible"));
  fr->insert(QTranslatorMessage("ViewFileManager","Incompatible init file version","","La version du fichier d'initialisation est incompatible"));
  fr->insert(QTranslatorMessage("ViewFileManager","Version","","Version"));
  fr->insert(QTranslatorMessage("ViewFileManager","Current Version","","Version Courante"));
  fr->insert(QTranslatorMessage("ViewFileManager","Init file","","Fichier d'initialisation"));
  fr->insert(QTranslatorMessage("ViewFileManager","Cannot access to init file","","Fichier d'initialisation inaccessible"));
  fr->insert(QTranslatorMessage("ViewFileManager","Cannot access to existing init file","","Fichier d'initialisation existant inaccessible"));
  
  fr->insert(QTranslatorMessage("ViewLocationBar"," Location ",""," Adresse "));
  fr->insert(QTranslatorMessage("ViewLocationBar","Erase Location","","Efface l'Adresse"));
  fr->insert(QTranslatorMessage("ViewLocationBar","The Filename","","Le Nom de fichier courant"));

  fr->insert(QTranslatorMessage("ViewHelpMenu","What's &This?","","Qu'est ce que c'est?"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","&Help","","Aide"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","&About Viewer","","&A propos du Viewer"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","&License","","&Licence"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","About &Qt","","A propos de &Qt"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","About Qt","","A propos de Qt"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Technical Characteristics","","Caractéristiques techniques"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Technical Characteristics","","Caractéristiques techniques"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Qt Hierarchy","","Hiérarchie Qt"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Style","","Style"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","How to use Viewer","","Utilisation du Visualisateur"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","PlantGL Viewer","","Visualisateur PlantGL"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Version","","Version"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Geom Library","","Bibliotheque GEOM"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Binary Format Version","","Version du Format Binaire"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Real Type Precision","","Précision du type Real"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Double","","Double"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Simple","","Simple"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Using Threads","","Utilisation des Threads"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Geom Namespace","","Espace de Nom GEOM"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","True","","Vrai"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","False","","Faux"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Geom Debug","","Geom Debug"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Geom DLL","","Geom DLL"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Using Glut","","Utilisation de Glut"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Tools Library","","Bibliothèque Tools"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Tools Namespace","","Espace de Nom Tools"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Using RogueWave","","Utilisation de RogueWave"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Install Path","","Repertoire d'installation"));
  fr->insert(QTranslatorMessage("ViewHelpMenu","Symbol Path","","Repertoire des Symboles"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Flex","","Flex"));
  // fr->insert(QTranslatorMessage("ViewHelpMenu","Amapmod","","Amapmod"));
  fr->insert(QTranslatorMessage("ViewSysInfo","&Save","","&Enregistrer"));
  fr->insert(QTranslatorMessage("ViewSysInfo","&Cancel","","&Annuler"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","&Ok","","&Ok"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Information","","Information"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Values","","Valeurs"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Machine","","Machine"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Processor","","Processeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Number of processor","","Nombre de Processeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Number of processor","","Nombre de Processeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","System","","Système"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Language","","Langue"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Version","","Version"));
  fr->insert(QTranslatorMessage("ViewSysInfo","True","","Vrai"));
  fr->insert(QTranslatorMessage("ViewSysInfo","False","","Faux"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Yes","","Oui"));
  fr->insert(QTranslatorMessage("ViewSysInfo","No","","Non"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Disable","","Désactivé"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Word Size","","Taille de mot"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Byte Order","","Ordre des bits"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Big Endian","","Big Endian"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Little Endian","","Little Endian"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Process","","Processus"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Id","","Id"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Compilation","","Compilation"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Date","","Date"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Compiled on","","Compilé sur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Process","","Processus"));
  fr->insert(QTranslatorMessage("ViewSysInfo","at","","à"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Build Mode","","Mode de compilation"));
  fr->insert(QTranslatorMessage("ViewSysInfo","C++ Compiler","","Compilation C++"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Run-Time Type Information","","Informations sur les types à l'execution"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Exception Handling","","Gestion des Exceptions"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Compilation Optimization","","Optimisation à la compilation"));
  fr->insert(QTranslatorMessage("ViewSysInfo","C++ Standard's Version","","Version du Standard C++"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Qt Library","","Bibliothèque Qt"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Thread Support","","Support des Threads"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Qt DLL","","Qt DLL"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Default Font","","Fonts par défaut"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Charset","","Charset"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Family","","Famille"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Size","","Taille"));
  fr->insert(QTranslatorMessage("ViewSysInfo","GL Widget","","Fenêtre GL"));
  fr->insert(QTranslatorMessage("ViewSysInfo","GL Context","","Contexte GL"));
  fr->insert(QTranslatorMessage("ViewSysInfo","GL Format","","Format GL"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Default","","Par Defaut"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Valid","","Valide"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Shared","","Partagé"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Direct Rendering","","Rendu Direct"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Double Buffer","","Tampon Double"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Depth Buffer","","Tampon de Profondeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Alpha channel","","Couleur Alpha"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Accumulation buffer","","Tampon d'Accumulation"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Stencil buffer","","Tampon de tracé"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Stereo buffering","","Stockage en Stéréo"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Overlay Plane","","Plan de Recouvrement"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Plane","","Plan"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Overlay GL Context","","Contexte GL de Recouvrement"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Overlay GL Format","","Format GL de Recouvrement"));
  fr->insert(QTranslatorMessage("ViewSysInfo","None","","Aucun"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Vendor","","Vendeur"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Client","","Client"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Renderer","","Renderer"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Extension(s)","","Extension(s)"));
  fr->insert(QTranslatorMessage("ViewSysInfo","OpenGL Utility Library (GLU)","","Bibliothèque OpenGL Utility (GLU)"));
  fr->insert(QTranslatorMessage("ViewSysInfo","OpenGL Utility Toolkit Library (Glut)","","Bibliothèque OpenGL Utility Toolkit (Glut)"));
  fr->insert(QTranslatorMessage("ViewSysInfo","OpenGL Windows Extension (WGL)","","Extension OpenGL de Windows (WGL)"));
  fr->insert(QTranslatorMessage("ViewSysInfo","OpenGL X Extension (GLX)","","Extension OpenGL de X (GLX)"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Screen","","Ecran"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Screens","","Ecrans"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Number of Screen","","Nombre d'Ecran"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Server","","Serveur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Display Name","","Nom de l'affichage"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Revision","","Révision"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Resolution","","Résolution"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Default Depth","","Profondeur par défaut"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Number of Entries in Default Colormap","","Taille de la carte de couleur par défaut"));
//  fr->insert(QTranslatorMessage("ViewSysInfo","Backing Store","","Backing Store"));
  fr->insert(QTranslatorMessage("ViewSysInfo","When Mapped","","En cas de mapping"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Not Useful","","Pas Utile"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Always","","Toujours"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Save Unders","","Save Unders"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Supported","","Supporté"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Not Supported","","Non Supporté"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Connection Number","","Numero de Connection"));
  // fr->insert(QTranslatorMessage("ViewSysInfo","Format","","Format"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Pixmap Format","","Format de Pixmap"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Depth","","Profondeur"));
  fr->insert(QTranslatorMessage("ViewSysInfo","Bits per pixel","","Bites par pixel"));

  fr->insert(QTranslatorMessage("ViewGLFrame","Set Background Color to","","Couleur de Fond à"));
  fr->insert(QTranslatorMessage("ViewGLFrame","GL Error","","Erreur GL"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Abort","","Abandonner"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Continue","","Continuer"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Set Line Width to","","Epaisseur de Ligne assignée à"));
  fr->insert(QTranslatorMessage("ViewGLFrame"," Line Width ",""," Epaisseur Ligne "));
  fr->insert(QTranslatorMessage("ViewGLFrame","Line Width","","Epaisseur Ligne"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Mode Multiple Selection","","Mode Sélection Multiple"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Mode Selection","","Mode Sélection"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Selection cleared","","Sélection effacée"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Clear Selection","","Effacer la Sélection"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Selection","","Sélection"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Rectangle Selection","","Sélection Rectangulaire"));
  // fr->insert(QTranslatorMessage("ViewGLFrame","Rectangle","","Rectangule"));
  // fr->insert(QTranslatorMessage("ViewGLFrame","Point","","Point"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Mouse on","","Pointeur en"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Selection from","","Selection de"));
  fr->insert(QTranslatorMessage("ViewGLFrame","to","","à"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Save screenshot with format","","Enregistre la capture d'écran au format"));
  fr->insert(QTranslatorMessage("ViewGLFrame","in","","sous"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Copy screenshot to clipboard","","Copie la capture d'écran dans le presse papier"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Cannot access global clipboard","","Impossible d'acceder au presse papier global"));
  fr->insert(QTranslatorMessage("ViewGLFrame","System Error","","Erreur Système"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Edit Line Width","","Editer l'épaisseur de ligne"));
  // fr->insert(QTranslatorMessage("ViewGLFrame","Renderer","","Renderer"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Camera","","Caméra"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Light","","Lumière"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Fog","","Brouillard"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Grid","","Grille"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Clipping Plane","","Plan de Coupe"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Rotating Center","","Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Background Color","","Couleur de fond"));
  fr->insert(QTranslatorMessage("ViewGLFrame","GL Options","","Options GL"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Culling","","Elimination des faces"));
  fr->insert(QTranslatorMessage("ViewGLFrame","None","","Aucunes"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Back Face","","Faces Arrières"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Front Face","","Faces Avants"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Shade Model","","Modèle d'Ombrage"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Flat","","Plat"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Smooth","","Souple"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Color/Material Dithering","","Couleur/Matériel Dithering"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Hidden Surface Removal","","Elimination des surfaces cachées"));
  fr->insert(QTranslatorMessage("ViewGLFrame","Normals Normalization","","Normalisation des Normales"));

  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Open &Geom File","","Ouvrir un Fichier &Geom"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","&Add Geom File","","&Ajouter un Fichier Geom"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Open &2 Geom File","","Ouvrir 2 Fichiers Geom"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open &Geom File","","Ouvrir un Fichier &Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","&Add Geom File","","&Ajouter un Fichier Geom"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Save As &Geom","","Enregistrer au Format &Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Save &Selection","","Enregistrer la Sélection"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Save &Not Selection","","Enregistrer Tout sauf la Sélection"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Import &AmapSymbol","","Importer un Symbole &Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Import &Linetree Files","","Importer une &Ligne Elastique"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Import &GeomView Files","","Importer un Fichier &GeomView"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Import &VegeStar Files","","Importer un Fichier &VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Import VegeStar Symbol","","Importer les Symboles &VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Import Symbol","","Importer le Symbole"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Show Symbol","","Afficher le Symbole"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Set Selection as Symbol","","Assigner la Sélection au Symbole"));

  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Export as &AmapSymbol","","Exporter en Symbole &Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Export as &Linetree","","Exporter en &Ligne Elastique"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Export as Pov&Ray","","Exporter en Pov&Ray"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Export as Vr&ml","","Exporter en Vr&ml"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Export as Ply","","Exporter en Ply"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Export as V&egeStar","","Exporter en V&egeStar"));

  fr->insert(QTranslatorMessage("ViewImporterSelection","File Format not recognized !","","Format de Fichier non reconnu!"));
  fr->insert(QTranslatorMessage("ViewImporterSelection","File :","","Fichier:"));
//  fr->insert(QTranslatorMessage("ViewImporterSelection","Type :","","Type :"));
  fr->insert(QTranslatorMessage("ViewImporterSelection","Choose Importer :","","Importer avec :"));
  fr->insert(QTranslatorMessage("ViewImporterSelection","&Cancel","","&Annuler"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Filename","","Nom de Fichier Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Cannot open Empty filename","","Impossible d'ouvrir un Nom de Fichier Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Already Reading File","","Lecture de Fichier en cours"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Currently Reading File","","En cours de Lecture du Fichier"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Already Reading File","","Lecture de Fichier en cours"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Currently Reading File","","En cours de Lecture du Fichier"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open GEOM File","","Ouvrir Fichier GEOM"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Add GEOM File","","Ajouter Fichier GEOM"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Geom File","","Fichier Geom"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Open GEOM File","","Ouvrir Fichier GEOM"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Geom File","","Fichier Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Amap Symbol","","Symbole Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open Amap Symbol","","Ouvrir Symbole Amap"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","VegeStar File","","Fichier VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open VegeStar File","","Ouvrir Fichier VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open VegeStar File","","Ouvrir Fichier VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","VegeStar Symbol","","Symbole VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open VegeStar Symbol","","Ouvrir Symbole VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Open VegeStar Symbol","","Ouvrir Symbole VegeStar"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Selection","","Sélection Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","No Shape are selected!","","Aucune Forme Sélectionnée!"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Separated File for","","Différents Fichiers pour"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Geometry","","Géométrie"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Appearance","","Apparence"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Save","","Enregistrer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","File Already Exists","","Fichier existant"));
  fr->insert(QTranslatorMessage("ViewFileManager","Shape File","","Fichier Forme"));
  fr->insert(QTranslatorMessage("ViewFileManager","Geometry File","","Fichier Géométrie"));
  fr->insert(QTranslatorMessage("ViewFileManager","Appearance File","","Fichier Appearance"));
  fr->insert(QTranslatorMessage("ViewFileManager"," already exists. Overwrite ?",""," existe déjà. Ecraser ?"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Yes","","Oui"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Yes To All","","Toujours Oui"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Cancel","","Annuler"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","File Generated with PlantGL 3D Viewer","","Fichier généré par PlantGL 3D Viewer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","PovRay File","","Fichier PovRay"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Cfg File","","Fichier Cfg"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Cannot write file","","Impossible d'écrire dans le fichier"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Vrml File","","Fichier Vrml"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Ply Format","","Format Ply"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Ply File","","Fichier Ply"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Binary Little Endian","","Binaire Little Endian"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Binary Big Endian","","Binaire Big Endian"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","GEOM Error","","Erreur GEOM"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Scene","","Scene Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Scene to Add","","Scene à Ajouter Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Scene Not Valid","","Scene Invalide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","To continue can cause dysfunction of this program","","Continuer peut causer un dysfonctionnement de ce programme"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","File","","Fichier"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Line","","Ligne"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Abort","","Annuler"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Continue","","Continuer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Validity","","Validité"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Display","","Afficher"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","geometric shapes.","","formes géométriques"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Display empty scene.","","Afficher Scene Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Shape","","Forme"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","unselected","","desélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","selected","","sélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","and","","et"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","shape selected","","forme sélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","shapes selected","","formes sélectionnées"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","shape unselected","","forme desélectionnée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","shapes unselected","","formes desélectionnées"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","not found","","pas trouvée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","not found","","pas trouvée"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Selection. Cannot Remove!","","Sélection vide. Impossible de Supprimer!"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Confirmation","","Confirmation"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Remove Selection?","","Supprimer la Sélection?"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Selection. Cannot Replace!","","Sélection vide. Impossible de Remplacer!"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Remove Selection","","Supprimer la Sélection"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Keep Selection Only","","Garder Seulement la Sélection"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Replace Selection by","","Remplacer la Sélection par"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Wire","","Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Discretization","","Discretisation"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","General Properties","","Propriétés Générales"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Number of Element","","Nombre d'Elément"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Number of Element","","Nombre d'Elément"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","shape(s)","","forme(s)"));
//  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Surface","","Surface"));
//  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Volume","","Volume"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Number of Polygon","","Nombre de Polygone"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Memory Size","","Taille Mémoire"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Memory Size","","Taille Mémoire"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Bounding Box","","Boîte Englobante"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Upper Rigth Corner","","Sommet Haut Droit"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Lower Left Corner","","Sommet Bas Gauche"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Lower Left Corner","","Sommet Bas Gauche"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Size","","Taille"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Center","","Centre"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Empty Scene","","Scene Vide"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Geom &Scene","","&Scene Geom"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Per Vertex","","Par Sommet"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Per Face","","Par Face"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","&Normal","","&Normale"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ViewGeomSceneGL","Recompute","","Recalculer"));
  // fr->insert(QTranslatorMessage("ViewGeomSceneGL","&Display List","","&Display List"));
  // fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Transition","","Transition"));
  fr->insert(QTranslatorMessage("ViewMultiGeomSceneGL","Transition Slider","","Slider de Transition"));
  fr->insert(QTranslatorMessage("ViewEditMatDialog","&Apply","","&Appliquer"));
  fr->insert(QTranslatorMessage("ViewEditMatDialog","&Reset","","&Réinitialiser"));
//  fr->insert(QTranslatorMessage("ViewEditMatDialog","&Ok","","&Ok"));
  fr->insert(QTranslatorMessage("ViewEditMatDialog","&Cancel","","A&nnuler"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Edit Material","","Editer le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Dissociate Material","","Dissocier le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Copy Material","","Copier le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Paste Material","","Coller le Matériel"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Material edition can be apply only on selected shapes.","","L'Edition de Matériel s'applique seulement sur les formes sélectionnées."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Material edition can be apply on one material.","","L'Edition de Matériel s'applique seulement sur un Matériel."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","At least one shape must be selected to dissociate material.","","Au moins une forme doit être sélectionnée pour dissocier le matériel."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","The Material of the first shape will be copy on all selected shapes.","","Le matériel de la première forme va être copié sur toutes les formes sélectionnées."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","One shape must be selected to copy material.","","Une forme doit être sélectionnée pour copier le matériel."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","Cannot copy multiple Material.","","Impossible de copier plusieurs matériels."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","GEOM Error","","Erreur GEOM"));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","No material available.","","Aucun matériel disponible."));
  fr->insert(QTranslatorMessage("ViewEditGeomSceneGL","At least one shape must be selected to paste material.","","Au moins une forme doit être sélectionnée pour coller le matériel."));
  fr->insert(QTranslatorMessage("ViewMultiscaleEditGeomSceneGL","GEOM Error","","Erreur GEOM"));
  fr->insert(QTranslatorMessage("ViewMultiscaleEditGeomSceneGL","Fit Geometry","","Ajuster une forme"));
  fr->insert(QTranslatorMessage("ViewMultiscaleEditGeomSceneGL","No Geometry to Fit.","","Aucune Géométrie à Ajuster."));
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
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Volume Rendering","","Rendu Volumique"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Volume and Wire Rendering","","Rendu Volumique et Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Wire Rendering","","Rendu Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Skeleton Rendering","","Rendu du Squelette"));
  // fr->insert(QTranslatorMessage("ViewModalRendererGL","Volume","","Volume"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Volume and Wire","","Volume et Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Wire","","Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Skeleton","","Squelette"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Control Points","","Points de Contrôle"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Control Points Rendering","","Rendu des Points de Contrôle"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Control Points Rendering Enable","","Rendu des Points de Contrôle Activé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Control Points Rendering Disable","","Rendu des Points de Contrôle Désactivé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Bounding Box","","Boîtes Englobantes"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Bounding Box Rendering","","Rendu des Boîtes Englobantes"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Bounding Box Rendering Enable","","Rendu des Boîtes Englobantes Activé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Bounding Box Rendering Disable","","Rendu des Boîtes Englobantes Désactivé"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Light Rendering","","Rendu avec Lumière"));
  fr->insert(QTranslatorMessage("ViewModalRendererGL","Light","","Lumière"));
//  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","&Volume","","&Volume"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","&Wire","","&Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","S&keleton","","S&quelete"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","Volu&me and Wire","","Volu&me et Fil de Fer"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","&Control Points","","Points de &Contrôle"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","&Bounding Box","","&Boîtes Englobantes"));
  fr->insert(QTranslatorMessage("ViewRenderingModeMenu","&Light","","&Lumière"));
//  fr->insert(QTranslatorMessage("ViewCameraMenu","&Home","","&Home"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Front View (YZ)","","Vue de &Face (YZ)"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Right View (XZ)","","Vue de &Droite (XZ)"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Top View (XY)","","Vue de De&ssus (XY)"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","GEOM System","","Système GEOM"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","GL System","","Système GL"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Change","","&Changer"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","Coordinates System","","Système de Coordonnées"));
  // fr->insert(QTranslatorMessage("ViewCameraMenu","&Perspective","","&Perspective"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","Ort&hographic","","Ort&hographique"));
  // fr->insert(QTranslatorMessage("ViewCameraMenu","Projection","","Projection"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Save","","Enregi&strer"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Read","","Cha&rger"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Auto Fit to Window","","Auto-Ajustement à la Fenêtre"));
  fr->insert(QTranslatorMessage("ViewCameraMenu","&Fit to Window","","Ajuster à la Fenêtre"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Perspective Camera","","Caméra en Perspective"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Orthographic Camera","","Caméra Orthographique"));
  fr->insert(QTranslatorMessage("ViewCameraGL","GEOM Coordinates System","","Système de Coordonnées GEOM"));
  fr->insert(QTranslatorMessage("ViewCameraGL","GL Coordinates System","","Système de Coordonnées GL"));
  fr->insert(QTranslatorMessage("ViewCameraGL","&Camera","","&Caméra"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Home Position","","Position Home"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Camera Position","","Position Camera"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Front View (YZ)","","Vue de &Face (YZ)"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Right View (XZ)","","Vue de &Droite (XZ)"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Top View (XY)","","Vue de De&ssus (XY)"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Fit to Window","","Ajuster à la Fenêtre"));
  fr->insert(QTranslatorMessage("ViewCameraGL","File Access","","Accès Fichier"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Cannot Open File","","Impossible d'Ouvrir le Fichier"));
  fr->insert(QTranslatorMessage("ViewCameraGL","Abort","","Abandon"));
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
  fr->insert(QTranslatorMessage("ViewClippingPlaneGL","Clipping Planes Control","","Contrôle des Plans de Coupe"));
  fr->insert(QTranslatorMessage("ViewCPlaneMenu","Control","","Contrôle"));
  fr->insert(QTranslatorMessage("ViewCPlaneMenu","Plane","","Plan"));
  // fr->insert(QTranslatorMessage("ClippingPlaneWidget","&Ok","","&Ok"));
  // fr->insert(QTranslatorMessage("ViewLightMenu","&Home","","&Home"));
  fr->insert(QTranslatorMessage("ViewLightMenu","on X axis","","sur l'Axe X"));
  fr->insert(QTranslatorMessage("ViewLightMenu","on Y axis","","sur l'Axe Y"));
  fr->insert(QTranslatorMessage("ViewLightMenu","on Z axis","","sur l'Axe Z"));
  fr->insert(QTranslatorMessage("ViewLightMenu","Visible","","Visible"));
  fr->insert(QTranslatorMessage("ViewLightGL","Light Source Visible","","Source Lumineuse Visible"));
  fr->insert(QTranslatorMessage("ViewLightGL","Light Source Invisible","","Source Lumineuse Invisible"));
  fr->insert(QTranslatorMessage("ViewFogGL","Fog Control","","Contrôle du Brouillard"));
  fr->insert(QTranslatorMessage("FogWidget","Fog","","Brouillard"));
  fr->insert(QTranslatorMessage("FogWidget","Mode","","Mode"));
  fr->insert(QTranslatorMessage("FogWidget","Start","","Début"));
  fr->insert(QTranslatorMessage("FogWidget","End","","Fin"));
  fr->insert(QTranslatorMessage("FogWidget","Density","","Densité"));
  fr->insert(QTranslatorMessage("FogWidget","Color","","Couleur"));
  // fr->insert(QTranslatorMessage("FogWidget","Hints","","Hints"));
  fr->insert(QTranslatorMessage("ViewFogGL","Control","","Contrôle"));
  fr->insert(QTranslatorMessage("ViewFogGL","Enable","","Activé"));
  fr->insert(QTranslatorMessage("ViewGridMenu","XY Plane","","Plan XY"));
  fr->insert(QTranslatorMessage("ViewGridMenu","XZ Plane","","Plan XZ"));
  fr->insert(QTranslatorMessage("ViewGridMenu","YZ Plane","","Plan YZ"));
  fr->insert(QTranslatorMessage("ViewGridMenu","Axis","","Axes"));
  fr->insert(QTranslatorMessage("ViewGridGL","XY Plane","","Plan XY"));
  fr->insert(QTranslatorMessage("ViewGridGL","XZ Plane","","Plan XZ"));
  fr->insert(QTranslatorMessage("ViewGridGL","YZ Plane","","Plan YZ"));
  fr->insert(QTranslatorMessage("ViewGridGL","Axis","","Axes"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL","Rotating Center","","Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL","Visible Rotating Center","","Centre de Rotation Visible"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL","Enable Rotating Center",""," Activer le Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL","Center Rotating Center","","Centrer le Centre de Rotation"));
  fr->insert(QTranslatorMessage("ViewRotCenterGL","Center Rotating Center","","Centrer le Centre de Rotation"));
//  fr->insert(QTranslatorMessage("ViewRotCenterMenu","&Home","","&Home"));
//  fr->insert(QTranslatorMessage("ViewRotCenterMenu","&Visible","","&Visible"));
  fr->insert(QTranslatorMessage("ViewRotCenterMenu","&Center","","&Centrer"));
  fr->insert(QTranslatorMessage("ViewRotCenterMenu","&Enable","","&Activer"));
  fr->insert(QTranslatorMessage("ViewRotCenterMenu","&Control","","&Contrôle"));
  fr->insert(QTranslatorMessage("ViewProperties","Properties","","Propriétés"));
  fr->insert(QTranslatorMessage("ViewProperties","Name","","Nom"));
//  fr->insert(QTranslatorMessage("ViewProperties","Location","","Location"));
  fr->insert(QTranslatorMessage("ViewProperties","Size","","Taille"));
  fr->insert(QTranslatorMessage("ViewProperties","Owner","","Propriétaire"));
  fr->insert(QTranslatorMessage("ViewProperties","Last Modified","","Dernière Modif."));
  fr->insert(QTranslatorMessage("ViewProperties","Last Accessed","","Dernièr Accès"));
  fr->insert(QTranslatorMessage("ViewProperties","&File","","&Fichier"));
  // fr->insert(QTranslatorMessage("ViewProperties","C&onfig","","C&onfig"));
  fr->insert(QTranslatorMessage("ViewProperties","Saved Options","","Options Sauvegardées"));
  fr->insert(QTranslatorMessage("ViewProperties","Window Position and Size","","Position et Taille de Fenêtre"));
  fr->insert(QTranslatorMessage("ViewProperties","Window Style","","Style de Fenêtre"));
  fr->insert(QTranslatorMessage("ViewProperties","ToolBars States (Experimental)","","Etats des Barres d'Outils (Experimental)"));
  fr->insert(QTranslatorMessage("ViewProperties","File History","","Historique des Fichiers"));
  fr->insert(QTranslatorMessage("ViewProperties","BackGround Color","","Couleur de Fond"));
  fr->insert(QTranslatorMessage("ViewProperties","Grids Visibility","","Visibilité des Grilles"));
  fr->insert(QTranslatorMessage("ViewProperties","Camera/Grid Automatic Fitting","","Ajustement Automatique des Caméra/Grilles"));
  fr->insert(QTranslatorMessage("ViewProperties","SpinBox instead of Dials in Control Panel","","SpinBox à la place des Dials"));
  fr->insert(QTranslatorMessage("ViewProperties","Appearance Options","","Options d'Apparence"));
  fr->insert(QTranslatorMessage("ViewProperties","use SpinBox instead of Dials in Control Panel","","Utilisation de SpinBox à la place des Dial"));
  fr->insert(QTranslatorMessage("ViewProperties","show Initialization Dialog at Startup","","Montrer la fenêtre d'Initialisation au démarage"));
  fr->insert(QTranslatorMessage("ViewProperties","Language","","Langue"));
  fr->insert(QTranslatorMessage("ViewProperties","Note: Language change takes effect only at next startup.","","Note: Modifier la langue ne prendra effet qu'au prochain démarrage"));

  fr->insert(QTranslatorMessage("ViewBrowser","Browser","","Explorateur"));
  fr->insert(QTranslatorMessage("QBrowser","Browser :","","Explorateur :"));
  fr->insert(QTranslatorMessage("QBrowser","Full &Mode","","&Mode Etendu"));
  fr->insert(QTranslatorMessage("QBrowser","&Cancel","","&Annuler"));
  // fr->insert(QTranslatorMessage("QBrowser","&Ok","","&Ok"));
  fr->insert(QTranslatorMessage("ViewErrorDialog","Viewer Error Dialog","","Fenêtre d'Erreur du Viewer"));
  fr->insert(QTranslatorMessage("MessageDisplayer","Verbose","","Bavard"));
  fr->insert(QTranslatorMessage("MessageDisplayer","Popup when Errors","","Apparaitre en cas d'erreurs"));
  fr->insert(QTranslatorMessage("MessageDisplayer","Cl&ear","","&Effacer"));
  fr->insert(QTranslatorMessage("MessageDisplayer","&Cancel","","&Annuler"));
  // fr->insert(QTranslatorMessage("MessageDisplayer","&Ok","","&Ok"));
  // fr->insert(QTranslatorMessage("MessageDisplayer","Message :","","Message :"));

  fr->insert(QTranslatorMessage("ViewControlPanel","Camera","","Caméra"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Elevation","","Elévation"));
  // fr->insert(QTranslatorMessage("ViewControlPanel","Azimuth","","Azimuth"));
  // fr->insert(QTranslatorMessage("ViewControlPanel","Zoom","","Zoom"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Move","","Pas"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Light Position","","Position Lumière"));
//  fr->insert(QTranslatorMessage("ViewControlPanel","Distance","","Distance"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Light Material","","Matériel Lumière"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Ambient","","Ambiant"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Diffuse","","Diffus"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Specular","","Spéculaire"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Grids","","Grilles"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Unit","","Unité"));
  fr->insert(QTranslatorMessage("ViewControlPanel","Size","","Taille"));

  fr->insert(QTranslatorMessage("QColorDialog","Cancel","","Annuler"));
  fr->insert(QTranslatorMessage("QColorDialog","&Add To Custom Colors","","Ajouter au Couleurs Personnalisées"));
  fr->insert(QTranslatorMessage("QColorDialog","&Custom colors","","&Couleurs Personnalisées"));
  fr->insert(QTranslatorMessage("QColorDialog","Select color","","Selection de Couleur"));
  fr->insert(QTranslatorMessage("QColorDialog","&Basic colors","","Couleurs de &Base"));
  fr->insert(QTranslatorMessage("QColorDialog","&Define Custom Colors >>","","Définit comme Couleur Personnalisée >>"));
  fr->insert(QTranslatorMessage("QColorDialog","&Red:","","&Rouge:"));
  fr->insert(QTranslatorMessage("QColorDialog","&Green:","","&Vert:"));
  fr->insert(QTranslatorMessage("QColorDialog","Bl&ue:","","Ble&u:"));
  
  }

  qApp->installTranslator( fr );
 // qWarning("French Translator installed");
}

QStringList 
getAvailableLanguage(){
  QStringList l("English");
  l.append("French");
  return l;
}
#endif
/* ----------------------------------------------------------------------- */
