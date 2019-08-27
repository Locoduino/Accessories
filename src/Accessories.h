//-------------------------------------------------------------------
#ifndef __accessories_H__
#define __accessories_H__
//-------------------------------------------------------------------

/**	 @mainpage
A full explanation in French can be read at http://www.locoduino.org/spip.php?article178 .

The goal of this library is to command accessories of railroad modeling, but basically these are motors and lights
exactly with the same constraints and needs than in a lot of other hobbies...

Accessories can be driven by basic functions to move or set on or off, but it can be driven by events too,
almost exactly the same as Commanders library is able to raise.

Each kind of accessories or ports can be excluded from compilation, for memory problems purpose or for naming conflicts.
You can exclude some parts of library here, to avoid losing program and data memory on parts you don't use.
For example, if you don't want SERVO, just uncomment the line #define NO_SERVO by removing // at the beginning.

The Arduino IDE compiles all the files of the library 'src' directory even if its content is not used.
The result is, if an unused source file contains static declarations (SERVO library do !), these statics
will be allocated and this memory will be lost. The only solution I have found is to rename the source files to
something IDE don't know...
So if you want to lose less memory, you can rename the linked files from .cpp to .ori, and from .hpp to .hppori.
See below in the exclusion area the file names related to each exclusion.

This software is Copyright (C) 2015-2018 thierry Paris / Locoduino. Use is subject to license
conditions. The main licensing options available are GPL V2 or Commercial:

\par Open Source Licensing GPL V2
This is the appropriate option if you want to share the source code of your
application with everyone you distribute it to, and you also want to give them
the right to share who uses it. If you wish to use this software under Open
Source Licensing, you must contribute all your source code to the open source
community in accordance with the GPL Version 2 when your application is
distributed. See http://www.gnu.org/copyleft/gpl.html

\par Commercial Licensing
This is the appropriate option if you are creating proprietary applications
and you are not prepared to distribute and share the source code of your
application. Contact info@open.com.au for details.

\page Revision History
\par 27/08/2019 V1.2.2
- Remplacement de LIBRARY_VERSION par ACCESSORIES_LIBRARY_VERSION
- Le mode debug avec ACCESSORIES_DEBUG_MODE est maintenant limité aux messages d'erreur. Si l'on veut plus, il faut activer aussi ACCESSORIES_DEBUG_VERBOSE_MODE
- Correction d'un problème de numéro d'expander quand ils sont activés dans Port.cpp
_______________
- LIBRARY_VERSION define has been replaced by ACCESSORIES_LIBRARY_VERSION
- The mode debug with ARDUINO_DEBUG_MODE is now limited to error messages. To see more, activate also ACCESSORIES_DEBUG_VERBOSE_MODE
- Fix of an expander number error in Port.cpp .

\par 08/07/2019 V1.2.1
- Correction de la présence de ARDUINO_DEBUG_MODe qui n'existe pas !
- Ajout d'un test dans Port::MovePin() pour identifier un GPIO_pin_t par rapport à un pin expander.
- Ajout d'un test au lancement pour vérifier que toutes les pins des expandeurs chargés ne rentrent pas en collision avec les GPIO_pin_t.
- Suppression de la fonction (dangereuse) Accessories::wait(). Elle faisit bien un loop de accessories, mais pas du reste du loop du .ino !
- Une fonction GetPinsNumber() a été ajoutée sur les expanders.
_______________
- ARDUINO_DEBUG_MODE, unknown define, has been removed.
- In Port::MovePin(), a test has been added to check the difference between expander pin and GPIO_pin_t pin.
- A new test has been added at the end of main setup() function to check compatibility between all expander pins and GPIO_pin_t.
- The function Accessories::wait() has benn removed. It was dangerous to use it because only the Accessories::loop() were called, and not all the stuff in the .ino main loop() !
- A new virtual function GetPinsNumber() has been added to the expander class.

\par 31/05/2019 V1.2.0
- Ajout des PortExpanders
- Intégration de la nouvelle version d'ArduiEmulator.
- Création d'un projet autonome à utiliser dans une solution VS2019.
_______________
- Add of PortExpanders
- Integration of the new version of the ArduiEmulator.					 
- Creation of a stand alone project to insert in VS2019 solutions.

\par 15/07/2018 V1.1.4
- Correction du format de Keywords.txt
- Création de LIBRARY_VERSION
- Mise en conformité de la partie doc HTML avec DOXYGEN_SPECIFIC
- Ajout du fichier LICENSE
_______________
- Fix Keywords.txt format.
- LIBRARY_VERSION created
- Update of DOXYGEN_SPECIFIC part for HTML documentation.
- Add of LICENSE file

\par 26/05/2018 V1.1.3
- Correction de perte mémoire dans ActionsStack (merci encore lapive).
_______________
- Fix memory leak in ActionsStack.

\par 15/05/2018 V1.1.2
- Correction de debounceDelay qui restait non initialisée (merci lapive).
_______________
- Fix debounceDelay value not initialized.

\par 17/04/2018 V1.1.1
- Correction des pb de compilation de AccessoryStepper.
- Correction des pb de compilation de PortShieldL293d.
_______________
- Fix compilation problems of AccessoryStepper.
- Fix compilation problems of PortShieldL293d.

\par 10/04/2018 V1.1.0
- Correction des pb de compilation et EEPROM si NO_GROUP est défini.
- Ajout de AddMovingPositions() pour des MovingPositions déclarées en PROGMEM
- Transformation de CanMove() et IsMovementPending() plus adapté.
- Ajout d'un événement ACCESSORIES_EVENT_SETDURATION.
- AccessoryBaseLight : ajout du fading physiologique !
- AccessoryBaseLight : le clignotement a été corrigé.
- Ajout dans la doc de la liste des événements avec leur effet pour chaque accessoire.
- AccessoryMultiLight : Traitement correct des événements avec application à toutes les leds de l'accessoire.
- AccessoryMotor : Ajout du traitement de l'événement ACCESSORIES_EVENT_SETDURATION.
- AccessortServo : Ajout du traitement des événements ACCESSORIES_EVENT_SETDURATION et ACCESSORIES_EVENT_EXTERNALMOVE.
- AccessoryServo : Correction des mouvements à faible vitesse
- AccessoryStepper : Ajout du traitement de l'événement ACCESSORIES_EVENT_SETDURATION.
- Port : Gain mémoire par fusionnement des deux attributs dans un seul octet.
- PortServo : MoveStop fait maintenant un detach() du servo.
- PortServo : MovePosition fait un attach du servo s'il n'est pas attaché.
- PortShieldL293d : Correction des pb de compilation si NO_SHIELDL293D est défini
- Ajout d'un exemple utilisant Commanders::EventsSequencer.
_______________
- Fix compilation and EEPROM problems if NO_GROUP is defined.
- Added AddMovingPositions() for MovingPositions declared in PROGMEM.
- Change of Accessory::CanMove() to IsMovementPending() more pertinent.
- Added the event ACCESSORIES_EVENT_SETDURATION.
- AccessoryBaseLight : use of physiologic fading !
- AccessoryBaseLight : blinking has been fixed.
- In HTML documentation, adding of tables of events and their effects for each kind of accessory.
- AccessoryMultiLight : correct handling of events with application on all leds of the accessory.
- AccessoryMotor : Added handling of ACCESSORIES_EVENT_SETDURATION event.
- AccessortServo : Added handling of ACCESSORIES_EVENT_SETDURATION and ACCESSORIES_EVENT_EXTERNALMOVE events.
- AccessoryServo : Fix slow moves.
- AccessoryStepper : Added handling of ACCESSORIES_EVENT_SETDURATION event.
- Port : Memory gained by using on byte for two flags.
- PortServo : MoveStop do now a detach() of the servo.
- PortServo : MovePosition do an attach() of the servo if not already done.
- PortShieldL293d : Fix compilation problem if NO_SHIELDL293D is defined.
- Added a new sample using Commanders::EventsSequencer.

\par 16/02/2018 V1.0.2
- Correction du clignotement dans AccessoryBaseLight
- Ajout de AccessoryLight::SetBlinking()
- Correction de la syntaxe de la documentation anglaise.
_______________
- Fix of blinking in AccessoryBaseLight
- Added AccessoryLight::SetBlinking()
- Fix of English documentation syntax !

\par 18/10/2017 V1.0.1
- Mise en forme du numéro de version en 'semver' (Semantic Versioning 2.0.0).
_______________
- Version number converted to 'semver' format (Semantic Versioning 2.0.0).

\par 17/10/2017 1.01
- Ajout de ArduiEmulator.zip dans extras
- Correction dans les moteurs qui ne bougaient plus.
_______________
- ArduiEmulator.zip added in extras.
- Fix of AccessoryMotor moving...

\par 25/09/2017 1.0
- Correction de l'allumage des leds en fading qui ne marchait plus...
- Ajout de Port::GetPinType()
- Test dans AccessoryBaseLight::SetFading() que le port soit bien de type ANALOG.
_______________
- Fix of light with fading activated.
- Function Port::GetPinType() added.
- Check of pin type which must be ANALOG or ANALOG_INVERTED for fading lights.

\par 23/09/2017 0.71
- Correction de l'allumage des leds qui ne marchait plus...
_______________
- Fix of light turning on or off.

\par 25/08/2017 0.70
- Nouvelle doc accessible par StartDoc.bat ou extras/Doc/index.html
- Passage en private de quantité de données et de fonctions.
- Tranfert de beaucoup de fonctions statiques de Accessories vers Accessory.
- L'argument inBlinkMask de AccessoryMultiLight::AddMovingPosition a maintenant une valeur par défaut de 0. 0 : pas de clignotement.
- Correction de AccessoryServo::IsMiscPosition() .
- Correction de PortSpeedDirBrake::IsActive()
- Suppression de Accessory::usestateNone et Accessory::type inutilisés.
- ActionStack::CheckIndex inutilisé, a disparu.
- positional -> positionnal !
- synchrone -> synchronous !
- AccessoryLight::IsFlashing -> AccessoryLight->IsBlinking !
- ActionStack::Purge -> ActionStack::Clear
- ActionStack::Purge(index) -> ActionStack::Delete(index)
- PortServo::GetPin2() -> PortServo::GetDIOPin()
_______________
- New doc available with StartDoc.bat or extras/Doc/index.html
- Lots of data and functions passed to private.
- Lots of static functions fro Accessories passed to Accessory.
- Argument inBlinkMask of AccessoryMultiLight::AddMovingPosition has now a default value of 0 (not blink)
- Fix of AccessoryServo::IsMiscPosition().
- Fix of PortSpeedDirBrake::IsActive()
- Accessory::usestateNone and Accessory::type unused, deleted.
- ActionStack::CheckIndex() unused, deleted.
- positionnal -> positional !
- synchrone -> synchronous !
- AccessoryLight::IsFlashing -> AccessoryLight->IsBlinking !
- ActionStack::Purge -> ActionStack::Clear
- ActionStack::Purge(index) -> ActionStack::Delete(index)
- PortServo::GetPin2() -> PortServo::GetDIOPin()

\par 15/02/2017 0.60
- Ajout de printMovingPositions()
- Ajout de ExternalMove() pour mettre en conformité l'état d'un moteur après une manoeuvre manuelle.
_______________
- printMovingPositions() added.
- ExternalMove() added to set the current state of a motor to its real state after a manual move.

\par 10/01/2017 0.51
- Correction de AccessoryLightMulti::AddMovingPosition().
_______________
- Fix AccessoryLightMulti::AddMovingPosition().

\par 27/12/2016 0.50
- Ajout de printAccessories()
- Correction du Event() de Light.
- Correction du MoveToggle de AccessoryMotorTwoWays
_______________
- printAccessories() added.
- Fix Event for lights
- Fix MoveToggle of AccessoryMotorTwoWays.

\par 04/12/2016 0.45
- Correction de la gestion de groupes.
- Correction de la vérification des ports au lancement
- Correction de Blink lorsqu'aucun delai n'a été fixé.
_______________
- Fix group event handling
- Fix port check at start.
- Fix Blink when no delay has been specified.

\par 22/11/2016 0.44
- Correction de AccessoryMotor qui ne tenait pas compte de la durée d'activation
_______________
- Fix of AccessoryMotor with activation duration used now.

\par 19/11/2016 0.43
- Passage en 'public' de toutes les fonctions de manipulations basiques des moteurs : MoveLeft, MoveRight, MoveStop, Toggle...
- Retrait de la compilation de CircularBuffer lorsque que NO_EEPROM est déclaré.
_______________
- All basic functions of motors have been passed in 'public'.
- Part of code of CircularBuffer removed when  NO_EEPROM is used.

\par 07/11/2016 0.41
- Correction des exemples
- Correction des exclusions #define NO_*
_______________
- Fix of samples.
- Fix exclusions with #define NO_*

\par 04/11/2016 0.40
- Utilisation d'un Circular Buffer local pour la sauvegarde EEPROM
_______________
- Use of a small version of CircularBuffer to write on EEPROM

\par 01/11/2016 0.30
- Disparition des tous les drivers ! Il n'y a plus que des ports...
- Ajout de la fonction Wait.
- Sauvegarde de l'état des accessoires dans l'EEPROM.
- Renommage de DEBUG_VERBOSE_MODE en ACCESSORIES_DEBUG_VERBOSE_MODE
_______________
- All 'Drivers' have been removed, replaced by Ports directly.
- Wait function added.
- Accessories states can be saved in EEPROM.
- Renaming of DEBUG_VERBOSE_MODE to ACCESSORIES_DEBUG_VERBOSE_MODE

\par 04/09/2016 0.20
- Plus d'initialisation de Serial, pour le DUE. Serial.begin() doit être fait dans le .ino .
- Ajout de l'événement ACCESSORIES_EVENT_MOVEPOSITIONID.
- un begin() ajouté pour Accessories.
- sur une (fausse) intuition, retrait de tous les 'byte' remplacés par des uint8_t .
- Ajout de AccessoryStepper
- Ajout de DriverLMD18200
- Ajout de DriverL9110
- Ajout de DriverULN2003
- Renommage de DriverPortRelay en DriverPortMotor
- Renommage de DriverPortL298n en DriverPort2Pins
- Renommage de DriverPortServoArduino en DriverPortServoBase
- Renommage de DEBUG_MODE en ACCESSORIES_DEBUG_MODE
- Correction de ActionStack pour les servos.
_______________
- No more internal begin() of Serial class, to avoid problems with the DUE.
- Serial.begin must be done, if necessary, in .ino file .
- Added event ACCESSORIES_EVENT_MOVEPOSITIONID.
- Added a begin() function for Accessories.
- Based on a false deduction, replacement of all 'byte' by 'uint8_t'. It is not wrong, but it could have been avoided...
- Added AccessoryStepper
- Added DriverLMD18200
- Added DriverL9110
- Added DriverULN2003
- Renaming of DriverPortRelay to DriverPortMotor
- Renaming of DriverPortL298n to DriverPort2Pins
- Renaming of DriverPortServoArduino to DriverPortServoBase
- Renaming of DEBUG_MODE to ACCESSORIES_DEBUG_MODE
- Fix ActionStack for servos.

\par 21/08/2016 V0.19
- Correction de la gestion de mémorisation d'événement durant un mouvement de servo.
_______________
- Fix event recording during a long servo movement.

\par 21/08/2016 V0.18
- Tous les exemples fonctionnent et ont été remaniés pour plus de clarté.
- Ajout de ACCESSORIES_EVENT_MOVEPOSITIONINDEX
- Ajout de Accessories::RaiseEvent()
- Ajout de UNDEFINED_POS
- Ajout de AccessoryMotor::beginTwoWays()
- Correction du clignotement dans AccessoryLightMulti
- Correction de la duree dans DriverPortL298n
_______________
- All samples are now fixed, and partially rewritten.
- Added ACCESSORIES_EVENT_MOVEPOSITIONINDEX
- Added Accessories::RaiseEvent()
- Added UNDEFINED_POS
- Added AccessoryMotor::beginTwoWays()
- Fix blinking in AccessoryLightMulti
- Fix duration in DriverPortL298n

\par 17/08/2016 V0.17
- Exemple SignalFrench corrigé
_______________
- SignalFrench fixed.

\par 16/08/2016 V0.16
- Encore des exemples corrigés
_______________
- More samples adapted.

\version 15/08/2016 V0.15
- Correction de AccessoryMotorTwoWays, de AccessoryServo, de DriverL293d et Driver L298n, et de certains exemples.
_______________
- Fix of AccessoryMotorTwoWays, of AccessoryServo, of DriverL293d and Driver L298n, and some samples.

\par 02/08/2016 V0.1
- Utilisation de la stratégie des événements déjà utilisée dans Commanders.
- Utilisation systématique des listes chaînées pour simplifier le codage en évitant de spécifier des tailles.
- Utilisation des constructeurs simples, et des fonction 'begin' pour construire et paramétrer les accessoires.
- Fonction loop simple qui gère tout !
_______________
- Use of event strategy already used in commanders.
- Use chained list to simplify user programming
- Use of default constructors to allow simple object declarations, and 'begin' functions to set good arguments.
- Easy and simple function 'loop' handling everything.

\par 01/08/2016 V0 Initial Release
- Accessories est né de la séparation de UniversalAccessoryDecoder en deux bibliothèques : Accessories et Commanders.
_______________
- Accessories is born, from the split of UniversalAccessoryDecoder V4 in two parts : Accessories and Commanders.
*/

/** @file Accessories.h
Main include file of the library.*/

#define ACCESSORIES_LIBRARY_VERSION		"Accessories V1.2.2"

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define ACCESSORIES_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if ACCESSORIES_DEBUG_MODE is not activated.
//#define ACCESSORIES_DEBUG_VERBOSE_MODE

///////////////////////////////////////////////////////
// The function Accessories::printAccessories()
// is very heavy in program memory. So to avoid problems
// you can make this function available by uncomment the next line.
//#define ACCESSORIES_PRINT_ACCESSORIES

/** This is used internally by DIO2.h */
#define  GPIO2_PREFER_SPEED    1

#include "DIO2.h"

/** Default size of action stack.*/
#define ACTION_STACK_SIZE	5

//////////////////////////////////////////
//  Exclusion area
//
//NO_GROUP
//	AccessoryGroup.cpp
//	AccessoryGroup.hpp
//
//NO_MOTOR
//	AccessoryMotor.cpp
//	AccessoryMotor.hpp
//	AccessoryMotorOneWay.cpp
//	AccessoryMotorOneWay.hpp
//	AccessoryMotorTwoWays.cpp
//	AccessoryMotorTwoWays.hpp
//	AFMotor.cpp
//	AFMotor.hpp
//
//NO_LIGHT
//	AccessoryBaseLight.cpp
//	AccessoryBaseLight.hpp
//	AccessoryLight.cpp
//	AccessoryLight.hpp
//	AccessoryLightMulti.cpp
//	AccessoryLightMulti.hpp
//
//NO_MOTOR_LIGHT
//	PortOnePin.cpp
//	PortOnePin.hpp
//	PortTwoPins.cpp
//	PortTwoPins.hpp
//	PortTwoPinsEnable.cpp
//	PortTwoPinsEnable.hpp
//	PortSpeedDirBrake.cpp
//	PortSpeedDirBrake.hpp
//	
//NO_SERVO
//	AccessoryServo.cpp
//	AccessoryServo.hpp
//	PortServo.cpp
//	PortServo.hpp
//	Servo.cpp
//	Servo.hpp
//
//NO_SHIELDL293D
//	PortShieldL293d.cpp
//	PortShieldL293d.hpp
//
//NO_STEPPER
//	AccessoryStepper.cpp
//	AccessoryStepper.hpp
//	DriverStepper.cpp
//	DriverStepper.hpp
//	PortStepper.cpp
//	PortStepper.hpp
//

//#define NO_GROUP
//#define NO_MOTOR
//#define NO_SERVO
//#define NO_STEPPER
//#define NO_LIGHT
//#define NO_SHIELDL293D
//#define NO_EXPANDER
//#define NO_EXPANDER_SX1509
//#define NO_EXPANDER_74HC595

#ifdef DOXYGEN_SPECIFIC
		// DO NOT CHANGE THESE LINES IN THIS BLOCK 'DOXYGEN_SPECIFIC' : Only here for documentation !

		/** If this is defined, groups part of the library is removed from the compilation. 
		It can result in a smaller memory footprint for the final program.*/
		#define NO_GROUP
		/** If this is defined, all the motors part of the library is removed from the compilation. 
		It can result in a smaller memory footprint for the final program.*/
		#define NO_MOTOR
		/** If this is defined, the servo part of the library is removed from the compilation. 
		It can result in a smaller memory footprint for the final program.*/
		#define NO_SERVO
		/** If this is defined, the stepper motor part of the library is removed from the compilation. 
		It can result in a smaller memory footprint for the final program.*/
		#define NO_STEPPER
		/** If this is defined, all the lights part of the library is removed from the compilation. 
		It can result in a smaller memory footprint for the final program.*/
		#define NO_LIGHT
		/** If this is defined, the Arduino Shield L293D part of the library is removed from the compilation. 
		It can result in a smaller memory footprint for the final program.*/
		#define NO_SHIELDL293D
		/** If this is defined, the state of all the library will not be saved to EEPROM.*/
		#define NO_EEPROM
		/** If this is defined, the expander sources will not be included in compilation.*/
		#define NO_EXPANDER
		/** If this is defined, the expander sources for SX1509 device will not be included in compilation.*/
		#define NO_EXPANDER_SX1509
		/** If this is defined, the expander sources for circuit 74HC595 device will not be included in compilation.*/
		#define NO_EXPANDER_74HC595

		#undef NO_GROUP
		#undef NO_MOTOR
		#undef NO_SERVO
		#undef NO_STEPPER
		#undef NO_LIGHT
		#undef NO_SHIELDL293D
		#undef NO_EEPROM

		/** If this is defined, the library will do many checks during setup and execution, and print errors, warnings and
		information messages on console. These messages can take a lot of memory, so be careful about the free memory of
		your program if you activate debug mode.*/
		#define ACCESSORIES_DEBUG_MODE
		/** If this is defined, the Verbose mode lets you see all actions done by the  library, but with a real flood of
		text to the console... It has no effect if ACCESSORIES_DEBUG_MODE is not activated.*/
		#define ACCESSORIES_DEBUG_VERBOSE_MODE
		/** If this is defined, the function Accessories::printAccessories() will become available. This is useful to try
		to understand why a port, or an accessory is not correctly defined.
		This function uses a lot of memory, so activate it only if necessary, and be careful about your program's memory.
		You can use the define PRINT_ACCESSORIES() in your sketch instead of a call to Accessories::printAccessories().
		If ACCESSORIES_PRINT_ACCESSORIES is not defined, PRINT_ACCESSORIES is defined as empty, so you will not have a compilation error.*/
		#define ACCESSORIES_PRINT_ACCESSORIES
#endif

#ifndef VISUALSTUDIO
#define NO_EEPROM
#endif

#ifdef NO_MOTOR
	#ifdef NO_LIGHT
		#define NO_MOTOR_LIGHT
	#endif
	#ifndef NO_MOTORONEWAY
		#define NO_MOTORONEWAY
	#endif
	#ifndef NO_MOTORTWOWAYS
		#define NO_MOTORTWOWAYS
	#endif
#endif

#ifdef NO_EXPANDER
#define NO_EXPANDER_SX1509
#define NO_EXPANDER_74HC595
#endif

// For Accessories library, L293D is not compatible with Arduino Due for the moment...
#ifdef ARDUINO_ARCH_SAM
#ifndef NO_SHIELDL293D
	#define NO_SHIELDL293D
#endif
#endif

/////////////////////////////////////

#include "Port.hpp"

#ifndef NO_MOTOR
#include "AccessoryMotorOneWay.hpp"
#include "AccessoryMotorTwoWays.hpp"
#endif
#ifndef NO_SERVO
#include "AccessoryServo.hpp"
#endif
#ifndef NO_STEPPER
#include "AccessoryStepper.hpp"
#endif
#ifndef NO_LIGHT
#include "AccessoryLight.hpp"
#include "AccessoryLightMulti.hpp"
#endif

#ifndef NO_GROUP
#include "AccessoryGroup.hpp"
#endif

#ifndef NO_MOTOR_LIGHT
#include "PortOnePin.hpp"
#include "PortTwoPins.hpp"
#include "PortTwoPinsEnable.hpp"
#include "PortSpeedDirBrake.hpp"
#endif

#ifndef NO_SHIELDL293D
#include "PortShieldL293d.hpp"
#endif

#ifndef NO_SERVO
#include "PortServo.hpp"
#endif

#ifndef NO_STEPPER
#include "PortStepper.hpp"
#endif

#ifndef NO_EXPANDER
	#include "PortExpander.hpp"
	#ifndef NO_EXPANDER_SX1509
		#include "PortExpanderSX1509.hpp"
	#endif
	#ifndef NO_EXPANDER_74HC595
		#include "PortExpander74HC595.hpp"
	#endif
#endif

#include "Accessories.hpp"
#endif
