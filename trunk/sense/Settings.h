//=============================================================================
// Copyright © 2005 Point Grey Research, Inc. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Point
// Grey Research, Inc. ("Confidential Information").  You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with Point Grey Research, Inc. (PGR).
// 
// PGR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. PGR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================
//=============================================================================
// $Id: Settings.h,v 1.1 2005/01/20 00:26:57 mwhite Exp $
//=============================================================================
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

//=============================================================================
// System Includes
//=============================================================================
#include <windows.h>

//=============================================================================
// PGR Includes
//=============================================================================
#include <digiclops.h>

//=============================================================================
// Project Includes
//=============================================================================


// extern void _main();
/**
 * This class handles saving/restoring Digiclops properties from the windows 
 * registry and/or an .ini file.
 */
class Settings  
{
public:

   /** @param context Must be an initialized Digiclops context. */
   Settings( DigiclopsContext context );
    
//    void setContext(DigiclopsContext context);
   
   virtual ~Settings();

   /** Save the current camera settings to an automatically generated
    *  registry key.*/
   bool saveToRegistry();

   /** Save the current camera settings to the indicated registry key. */
   bool saveToRegistry( const char* pszKey );

   /** Save the current camera settings to the indicated file. */
   bool saveToFile( const char* pszPath );

   /** Load settings and set camera based on settings from an automatically
    * generated registry key.*/
   bool loadFromRegistry();

   /** Load settings and set camera based on settings from the indicated
    *  registry key. */
   bool loadFromRegistry( const char* pszKey );

   /** Load settings and set camera based on settings from a file. */
   bool loadFromFile( const char* pszPath );


protected:

   struct Property
   {
      DigiclopsCameraProperty property;
      bool  bOnePush;
      bool  bOnOff;
      bool  bAuto;
      int   iValueA;
      int   iValueB;
      char  pszName[ 64 ];
   };

   Property m_arProperties[ DIGICLOPS_PROPERTY_TOTAL ];
   
   DigiclopsContext m_context;

   bool readPropertyFromCamera( Property* pproperty );
   bool writePropertyToCamera( const Property* pproperty );

   bool writePropertyToRegistry( HKEY key, const Property* pproperty );
   bool readPropertyFromRegistry( HKEY key, Property* pproperty );

   bool readAllProperties();
   bool writeAllProperties();

   bool getAutoRegistryKey( char* pszKey );

   bool getPropertyString( const Property* pproperty, char* pszString );
   bool scanPropertyString( Property* pproperty, const char* pszString );

   Property* findPropertyFromName( const char* pszPropertyName );
   
   bool handleDigiclopsError( 
      DigiclopsError error, const char* pszFunction, ... ) const;
};


#endif // #ifndef _SETTINGS_H_
