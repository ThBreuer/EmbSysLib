//*******************************************************************
/*!
\file   ExtInt.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_EXTINT_H
#define _HW_EXTINT_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class ExtInt

\brief ...

\example HwExtInt.cpp
*/
class ExtInt
{
  public:
    //---------------------------------------------------------------
    /*! \enum Edge
        \brief ...
    */
    typedef enum
    {
      NONE = 0,   //!< No Interrupt
      RISING,     //!< Interrupt with rising edge
      FALLING,    //!< Interrupt with falling edge
      BOTH        //!< Interrupt with both edges
    } Edge;

  public:
    //*******************************************************************
    /*!
    \class Task

    \brief Base class for external interrupt tasks
    */
    class Task : public Std::Sequence<Task>::Item
    {
      public:
        //---------------------------------------------------------
        /*! Method called by external interrupt.
            The application has to override this method.
        */
        virtual void update( void ) = 0;

    }; //class ExtInt::Task
    //*******************************************************************

  public:
    //---------------------------------------------------------------
    /*! Add a new object to the update-list.
        After executing this method, the objects update()-method
        will been called by external interrupt
        \param ptr Pointer to the object
    */
    virtual void add( Task *ptr );

    //---------------------------------------------------------------
    /*! ...
    */
    virtual void enable() = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual void disable() = 0;
    
    //---------------------------------------------------------------
    /*! ...
    */
    virtual void setEdge( Edge edge) = 0;

  protected:
    //---------------------------------------------------------------
    // Interrupt Service Routine of external interrupt
    // This method has to been called by the hardware driven
    // interrupt service routine only
    //
    virtual void isrHandler( void );

  private:
    //---------------------------------------------------------------
    Std::Sequence<Task> isrSeq; // list of callable objects

}; //class ExtInt

}  } //namespace

#endif
