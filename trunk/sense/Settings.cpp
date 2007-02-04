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
// $Id: Settings.cpp,v 1.1 2005/01/20 00:26:57 mwhite Exp $
//=============================================================================

//=============================================================================
// System Includes
//=============================================================================
#include <assert.h>
#include <cstdio>

//=============================================================================
// PGR Includes
//=============================================================================

//=============================================================================
// Project Includes
//=============================================================================
#include "Settings.h"


#define REGISTRY_KEY_TEXT "Software\\Point Grey Research, Inc.\\CameraSettings"



Settings::Settings( DigiclopsContext context )
{
   m_context = context;

   memset( m_arProperties, 0x0, sizeof( Property ) * DIGICLOPS_PROPERTY_TOTAL );

   m_arProperties[ DIGICLOPS_AUTO_EXPOSURE ].property = DIGICLOPS_AUTO_EXPOSURE;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_EXPOSURE ].pszName, "Auto Exposure" );

   m_arProperties[ DIGICLOPS_SHUTTER ].property = DIGICLOPS_SHUTTER;
   strcpy( m_arProperties[ DIGICLOPS_SHUTTER ].pszName, "Shutter" );

   m_arProperties[ DIGICLOPS_GAIN ].property = DIGICLOPS_GAIN;
   strcpy( m_arProperties[ DIGICLOPS_GAIN ].pszName, "Gain" );

   m_arProperties[ DIGICLOPS_DELTA_GAIN_TOP ].property = DIGICLOPS_DELTA_GAIN_TOP;
   strcpy( m_arProperties[ DIGICLOPS_DELTA_GAIN_TOP ].pszName, "Delta Gain Top" );

   m_arProperties[ DIGICLOPS_DELTA_GAIN_LEFT ].property = DIGICLOPS_DELTA_GAIN_LEFT;
   strcpy( m_arProperties[ DIGICLOPS_DELTA_GAIN_LEFT ].pszName, "Delta Gain Left" );

   m_arProperties[ DIGICLOPS_AUTO_GAIN_LOW ].property = DIGICLOPS_AUTO_GAIN_LOW;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_GAIN_LOW ].pszName, "Auto Gain Low" );

   m_arProperties[ DIGICLOPS_AUTO_GAIN_HIGH ].property = DIGICLOPS_AUTO_GAIN_HIGH;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_GAIN_HIGH ].pszName, "Auto Gain High" );

   m_arProperties[ DIGICLOPS_AUTO_SHUTTER_LOW ].property = DIGICLOPS_AUTO_SHUTTER_LOW;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_SHUTTER_LOW ].pszName, "Auto Shutter Low" );

   m_arProperties[ DIGICLOPS_AUTO_SHUTTER_HIGH ].property = DIGICLOPS_AUTO_SHUTTER_HIGH;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_SHUTTER_HIGH ].pszName, "Auto Shutter High" );

   m_arProperties[ DIGICLOPS_WHITEBALANCE ].property = DIGICLOPS_WHITEBALANCE;
   strcpy( m_arProperties[ DIGICLOPS_WHITEBALANCE ].pszName, "Software White Balance" );

   m_arProperties[ DIGICLOPS_HARDWARE_WHITEBALANCE ].property = DIGICLOPS_HARDWARE_WHITEBALANCE;
   strcpy( m_arProperties[ DIGICLOPS_HARDWARE_WHITEBALANCE ].pszName, "White Balance" );
}

// Settings::setContext(DigiclopsContext context)
// {
//    m_context = context;
// }
Settings::~Settings()
{
}


bool 
Settings::handleDigiclopsError( 
                               DigiclopsError   error, 
                               const char*      pszFunction, 
                               ... ) const
{
   if( error == DIGICLOPS_OK )
   {
      return true;
   }
   else
   {
      va_list ap;
      va_start( ap, pszFunction );
      
      char pszTemp[ 512 ];
      
      _vsnprintf( pszTemp, sizeof( pszTemp ), pszFunction, ap );
      
      OutputDebugString( pszTemp );
      fprintf( stderr, pszTemp );

      assert( false );
      
      va_end( ap );
      
      return false;
   }
}


bool 
Settings::saveToRegistry()
{
   char pszKey[ 512 ];
   
   if( getAutoRegistryKey( pszKey ) )
   {
      return saveToRegistry( pszKey );
   }
   else
   {
      return false;
   }
}


bool 
Settings::saveToRegistry( const char* pszKey )
{
   HKEY hkey;
   
   DWORD dwDisposition;
   
   if( RegCreateKeyEx(
      HKEY_CURRENT_USER,
      pszKey,
      0,
      REG_NONE,
      REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS,
      NULL,
      &hkey,
      &dwDisposition ) != ERROR_SUCCESS )
   {
      return false;
   }
   
   if( readAllProperties() )
   {
      for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
      {
         if( !writePropertyToRegistry( hkey, &m_arProperties[ i ] ) )
         {
            assert( false );
         }
      }
   }
   else
   {
      assert( false );
   }

   RegCloseKey( hkey );
   
   return true;
}


bool 
Settings::saveToFile( const char* pszPath )
{
   FILE* pfile = fopen( pszPath, "w" );
   
   if( pfile == NULL )
   {
      fprintf( stderr, "Could not open %s for writing.\n", pszPath );
      return false;
   }

   if( !readAllProperties() )
   {
      return false;
   }

   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      Property* pp = &m_arProperties[ i ];

      char pszPropertyString[ 512 ];

      if( getPropertyString( pp, pszPropertyString ) )
      {
         fprintf( pfile, "%s\n", pp->pszName );
         fprintf( pfile, "%s\n", pszPropertyString );
      }
   }

   fclose( pfile );

   return true;
}


bool 
Settings::loadFromRegistry()
{
   char pszKey[ 512 ];

   if( getAutoRegistryKey( pszKey ) )
   {
      return loadFromRegistry( pszKey );
   }
   else
   {
      return false;
   }
}


bool 
Settings::loadFromRegistry( const char* pszKey )
{
   HKEY hkey;

   DWORD dwDisposition;
   
   if( RegCreateKeyEx(
      HKEY_CURRENT_USER,
      pszKey,
      0,
      REG_NONE,
      REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS,
      NULL,
      &hkey,
      &dwDisposition ) != ERROR_SUCCESS )
   {
      return false;
   }

   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      if( readPropertyFromRegistry( hkey, &m_arProperties[ i ] ) )
      {
         writePropertyToCamera( &m_arProperties[ i ] );
      }
   }

   return true;
}


bool 
Settings::loadFromFile( const char* pszPath )
{
   FILE* pfile = fopen( pszPath, "r" );
   
   if( pfile == NULL )
   {
      fprintf( stderr, "Could not open %s for reading.\n", pszPath );
      return false;
   }
   
   char pszLine[ 512 ];
   
   while( fgets( pszLine, sizeof( pszLine ), pfile ) != NULL )
   {
      size_t posTemp = strcspn( pszLine, "\n" );

      if( posTemp > 0 )
      {
         pszLine[ posTemp ] = '\0';
      }

      Property* pproperty = findPropertyFromName( pszLine );      
      
      if( pproperty != NULL )
      {
         if( fgets( pszLine, sizeof( pszLine ), pfile ) != NULL )
         {       
            if( scanPropertyString( pproperty, pszLine ) )
            {
               if( !writePropertyToCamera( pproperty ) )
               {
                  assert( false );
                  return false;
               }
            }               
         }
      }
   }
   
   
   fclose( pfile );
   
   return true;
}


bool 
Settings::readPropertyFromCamera( Property* property )
{
   DigiclopsError error = digiclopsGetCameraPropertyEx(
      m_context,
      property->property,
      &property->bOnePush,
      &property->bOnOff,
      &property->bAuto,
      &property->iValueA,
      &property->iValueB );
      
   return handleDigiclopsError( error, "digiclopsGetCameraPropertyEx()" );
}


bool 
Settings::writePropertyToCamera( const Property* property )
{
   DigiclopsError error = digiclopsSetCameraPropertyEx(
      m_context,
      property->property,
      property->bOnePush,
      property->bOnOff,
      property->bAuto,
      property->iValueA,
      property->iValueB );

   if( error == DIGICLOPS_NOT_IMPLEMENTED )
   {
      // Ignore this error.
      return true;
   }
   else
   {
      return handleDigiclopsError( error, "digiclopsSetCameraPropertyEx()" );
   }
}


bool 
Settings::readAllProperties()
{
   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      readPropertyFromCamera( &m_arProperties[ i ] );
   }

   return true;
}


bool 
Settings::writeAllProperties()
{
   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      writePropertyToCamera( &m_arProperties[ i ] );
   }
   
   return true;
}


bool 
Settings::writePropertyToRegistry( HKEY key, const Property* pproperty )
{
   char pszValue[ 512 ];

   getPropertyString( pproperty, pszValue );

   return ( RegSetValueEx(
      key,
      pproperty->pszName,
      0,
      REG_SZ,
      (BYTE*)pszValue,
      strlen( pszValue ) ) == ERROR_SUCCESS );
}


bool 
Settings::readPropertyFromRegistry( HKEY key, Property* pproperty )
{
   char pszValue[ 512 ];
   DWORD dwMaxLen = sizeof( pszValue );
     
   if( RegQueryValueEx(
      key,
      pproperty->pszName,
      0,
      NULL,
      (BYTE*)pszValue,
      &dwMaxLen
      ) != ERROR_SUCCESS )
   {
      return false;
   }

   return scanPropertyString( pproperty, pszValue );
}


bool 
Settings::getAutoRegistryKey( char* pszKey )
{
   DigiclopsError    error;
   DigiclopsInfoEx   info;
   
   error = digiclopsGetCameraInfoEx( m_context, &info );
   if( !handleDigiclopsError( error, "digiclopsGetCameraInfoEx()" ) )
   {
      return false;
   }
   
   sprintf( pszKey, "%s\\%d", REGISTRY_KEY_TEXT, info.SerialNumber );
   
   return true;
}


bool 
Settings::getPropertyString( const Property* pproperty, char* pszString )
{
   sprintf( 
      pszString, 
      "ValueA=%d ValueB=%d OnePush=%d OnOff=%d Auto=%d",
      pproperty->iValueA,
      pproperty->iValueB,
      pproperty->bOnePush ? 1 : 0,
      pproperty->bOnOff ? 1 : 0,
      pproperty->bAuto ? 1 : 0 );

   return true;
}


bool 
Settings::scanPropertyString( Property* pproperty, const char* pszString )
{
   int iValueA;
   int iValueB;
   int iOnePush;
   int iOnOff;
   int iAuto;
   
   int iScan = sscanf(
      pszString,
      "ValueA=%d ValueB=%d OnePush=%d OnOff=%d Auto=%d",
      &iValueA,
      &iValueB,
      &iOnePush,
      &iOnOff,
      &iAuto );
   
   if( iScan == 5 )
   {
      pproperty->iValueA = iValueA;
      pproperty->iValueB = iValueB;
      pproperty->bOnePush = iOnePush != 0;
      pproperty->bOnOff = iOnOff != 0;
      pproperty->bAuto = iAuto != 0;
      
      return true;
   }
   else
   {
      return false;
   }
}


Settings::Property* 
Settings::findPropertyFromName( const char* pszPropertyName )
{
   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      if( _stricmp( pszPropertyName, m_arProperties[ i ].pszName ) == 0 )
      {
         return &m_arProperties[ i ];
      }
   }

   return NULL;
}
