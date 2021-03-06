/**
	* \file			ListView.h
	* \brief		Base class for all list views
	*
	*/

//= I N C L U D E S ====================================================
#include "ListView.h"
#include "DebugLogger.h"
#include "XBFont.h"

#include <string>

extern "C" {
  #include "osdepend.h"
}


//= D E F I N E S ======================================================


//= S T R U C T U R E S ===============================================

//= G L O B A L = V A R S ==============================================

//= P R O T O T Y P E S ================================================

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	MoveCursor
//---------------------------------------------------------------------
void CListView::MoveCursor( CInputManager &gp, BOOL useSpeedBanding )
{
		// General idea taken from XMAME
	static UINT64		lastTime = 0;
	UINT64 curTime = osd_cycles();
	m_timeElapsed = (FLOAT)(curTime - lastTime) / (FLOAT)osd_cycles_per_second();
	if( !lastTime )
	{
			// lastTime isn't valid yet, so wait for the next frame
		lastTime = curTime;
		return;
	}
	lastTime = curTime;

		// Decrement the dpad movement timer
	if( m_dpadCursorDelay > 0.0f )
	{
		m_dpadCursorDelay -= m_timeElapsed;
		if( m_dpadCursorDelay < 0.0f )
			m_dpadCursorDelay = 0.0f;
	}

		// General idea taken from XMAME
		// The combined trigger offset, scaled to the range [-1.0f,1.0f]
	FLOAT cursorVelocity =  ((FLOAT)gp.GetGamepad(0)->GetAnalogButtonState(GP_RIGHT_TRIGGER) - 
												  (FLOAT)gp.GetGamepad(0)->GetAnalogButtonState(GP_LEFT_TRIGGER)) / 256.0f;


  if( useSpeedBanding )
  {
		  // Reset the speed band timeout
	  if( cursorVelocity < 0.99f && cursorVelocity > -0.99f )
		  m_cursorSpeedBandTimeout = 0.0f;
	  else
	  {
		  m_cursorSpeedBandTimeout += m_timeElapsed;
		  if( m_cursorSpeedBandTimeout > SBTIMEOUT_FASTEST )
			  m_cursorSpeedBandTimeout = SBTIMEOUT_FASTEST;
	  }
  }

		// DPAD overrides the triggers
  if( (gp.IsOneOfButtonsPressed( GP_DPAD_DOWN | GP_LA_DOWN ) || gp.IsKeyPressed( VK_DOWN )) 
       && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that adding 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = 1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
  else if( (gp.IsOneOfButtonsPressed( GP_DPAD_UP | GP_LA_UP ) || gp.IsKeyPressed( VK_UP )) 
            && m_dpadCursorDelay == 0.0f )
	{
			// Round the cursor position down to a integer so that subtracting 1 will move to the next item
    m_pageOffset = (FLOAT)((LONG)m_pageOffset);
		m_cursorPosition = (FLOAT)((LONG)m_cursorPosition);
    cursorVelocity = -1.0f;
		m_dpadCursorDelay = DPADCURSORMOVE_TIMEOUT;
	}
	else
	{
			// Handle the dead zone
		if( cursorVelocity > -DEADZONE && cursorVelocity < DEADZONE )
				return;

			// Rescale the cursor velocity to be [-CURSOR_SPEED,CURSOR_SPEED] without the deadzone
    if( cursorVelocity > 0 )
      cursorVelocity -= DEADZONE;
    else
      cursorVelocity += DEADZONE;
		cursorVelocity *= DEADZONE_RECTIFIER;
    cursorVelocity *= CURSOR_SPEED;
	}



  if( useSpeedBanding )
  {
		  // Apply speed bands
	  if( m_cursorSpeedBandTimeout == SBTIMEOUT_FASTEST )
		  cursorVelocity *= SBMULTIPLIER_FASTEST;
	  else if( m_cursorSpeedBandTimeout > SBTIMEOUT_FASTER )
		  cursorVelocity *= SBMULTIPLIER_FASTER;
	  else if( m_cursorSpeedBandTimeout > SBTIMEOUT_FAST )
		  cursorVelocity *= SBMULTIPLIER_FAST;
  }

	DWORD pageSize = (m_numLinesInList < m_maxPageSize ? m_numLinesInList : m_maxPageSize);
	UINT32 pageHalfwayPoint = (pageSize >> 1);
	UINT32 maxPageOffset = m_numLinesInList - pageSize;



	if( cursorVelocity > 0 )
	{
			// Moving down in the list

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (UINT32)m_cursorPosition < pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_cursorPosition) < pageHalfwayPoint )
				m_cursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_cursorPosition);
				m_cursorPosition = (FLOAT)pageHalfwayPoint;
				m_pageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (UINT32)(cursorVelocity + m_pageOffset) <= maxPageOffset )
			{
				m_pageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_pageOffset) <= (FLOAT)maxPageOffset )
					m_cursorPosition += cursorVelocity;
				else
				{
					cursorVelocity -= ((FLOAT)maxPageOffset - m_pageOffset);
					m_pageOffset = (FLOAT)maxPageOffset;
					m_cursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (UINT32)m_pageOffset > maxPageOffset )
			m_pageOffset = (FLOAT)maxPageOffset;
		if( (UINT32)m_cursorPosition > (pageSize - 1) )
			m_cursorPosition = (FLOAT)(pageSize - 1);
	}
	else
	{
			//--- Moving up in the list -----------------------------------------------

			// If the cursor position is not locked at the halfway point, move it towards there
		if( (UINT32)m_cursorPosition > pageHalfwayPoint )
		{
				// See if the entire velocity is consumed in moving the cursor or not
			if( (cursorVelocity + m_cursorPosition) > pageHalfwayPoint )
				m_cursorPosition += cursorVelocity;
			else
			{
				cursorVelocity -= ((FLOAT)pageHalfwayPoint - m_cursorPosition);
				m_cursorPosition = (FLOAT)pageHalfwayPoint;
				m_pageOffset += cursorVelocity;
			}
		}
		else
		{
				// The cursor is already at the halfway point

				// If the page offset can be moved without going off the end of the list, do so
			if( (LONG)(cursorVelocity + m_pageOffset) >= 0 )
			{
				m_pageOffset += cursorVelocity;
			}
			else
			{
					// See if the entire velocity is consumed in moving the page or not
				if( (cursorVelocity + m_pageOffset) >= 0.0f )
					m_cursorPosition += cursorVelocity;
				else
				{
					cursorVelocity += m_pageOffset;
					m_pageOffset = 0.0f;
					m_cursorPosition += cursorVelocity;
				}
			}
		}

			// Cap values
		if( (LONG)m_pageOffset < 0 )
			m_pageOffset = 0.0f;
		if( (LONG)m_cursorPosition < 0 )
			m_cursorPosition = 0.0f;
	}
}





