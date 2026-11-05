///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/SerializeText.hpp>
#include <Langulus/Annies/Functional/Compare.hpp>
#include <Langulus/Annies/Functional/Describe.hpp>
#include <Langulus/Annies/Components/Stack.hpp>
#include <Langulus/Event.hpp>


namespace Langulus
{
   /// Descriptor-construction                                                
   Event::Event(Describe&& describe) : Event {} {
      LglsAssert(describe.ExtractData(*Get<DMeta, 1>()), "Invalid event");
      describe.ExtractData(*Get<EventState, 2>());
      describe.ExtractTag<Tags::Data>(*this);
      SetTimestamp(SteadyClock::Now());
   }

   bool Event::operator == (Event const& other) const noexcept {
      return this->CompareEqual(other);
   }

   /// Log an event                                                           
   /*Event::operator Text() const {
      if (mState) {
         if (mPayload)
            return Text::Text::Format("{}({}, {})", mType, mState, Tags::Data {mPayload});
         else
            return Text::Text::Format("{}({})", mType, mState);
      }
      else {
         if (mPayload)
            return Text::Text::Format("{}({})", mType, Tags::Data {mPayload});
         else
            return Text::Text::Format("{}()", mType);
      }
   }*/

   /// Register all commonly used event types                                 
   void RegisterEvents() {
      (void) MetaDataOf<Events::WindowClose>();
      (void) MetaDataOf<Events::WindowFileDrop>();
      (void) MetaDataOf<Events::WindowMouseHoverIn>();
      (void) MetaDataOf<Events::WindowMouseHoverOut>();
      (void) MetaDataOf<Events::WindowResolutionChange>();
      (void) MetaDataOf<Events::WindowMinimize>();
      (void) MetaDataOf<Events::WindowMaximize>();
      (void) MetaDataOf<Events::WindowFocus>();
      (void) MetaDataOf<Events::WindowUnfocus>();
      (void) MetaDataOf<Events::WindowResize>();
      (void) MetaDataOf<Events::WindowMove>();
      (void) MetaDataOf<Events::WindowText>();

      (void) MetaDataOf<Events::MouseMove>();
      (void) MetaDataOf<Events::MouseScroll>();

      (void) MetaDataOf<Keys::Space>();
      (void) MetaDataOf<Keys::Apostrophe>();
      (void) MetaDataOf<Keys::Comma>();
      (void) MetaDataOf<Keys::Minus>();
      (void) MetaDataOf<Keys::Period>();
      (void) MetaDataOf<Keys::Slash>();
      (void) MetaDataOf<Keys::Hack>();
      (void) MetaDataOf<Keys::Semicolon>();
      (void) MetaDataOf<Keys::LeftBracket>();
      (void) MetaDataOf<Keys::RightBracket>();
      (void) MetaDataOf<Keys::Tilde>();

      (void) MetaDataOf<Keys::Main0>();
      (void) MetaDataOf<Keys::Main1>();
      (void) MetaDataOf<Keys::Main2>();
      (void) MetaDataOf<Keys::Main3>();
      (void) MetaDataOf<Keys::Main4>();
      (void) MetaDataOf<Keys::Main5>();
      (void) MetaDataOf<Keys::Main6>();
      (void) MetaDataOf<Keys::Main7>();
      (void) MetaDataOf<Keys::Main8>();
      (void) MetaDataOf<Keys::Main9>();

      (void) MetaDataOf<Keys::A>();
      (void) MetaDataOf<Keys::B>();
      (void) MetaDataOf<Keys::C>();
      (void) MetaDataOf<Keys::D>();
      (void) MetaDataOf<Keys::E>();
      (void) MetaDataOf<Keys::F>();
      (void) MetaDataOf<Keys::G>();
      (void) MetaDataOf<Keys::H>();
      (void) MetaDataOf<Keys::I>();
      (void) MetaDataOf<Keys::J>();
      (void) MetaDataOf<Keys::K>();
      (void) MetaDataOf<Keys::L>();
      (void) MetaDataOf<Keys::M>();
      (void) MetaDataOf<Keys::N>();
      (void) MetaDataOf<Keys::O>();
      (void) MetaDataOf<Keys::P>();
      (void) MetaDataOf<Keys::Q>();
      (void) MetaDataOf<Keys::R>();
      (void) MetaDataOf<Keys::S>();
      (void) MetaDataOf<Keys::T>();
      (void) MetaDataOf<Keys::U>();
      (void) MetaDataOf<Keys::V>();
      (void) MetaDataOf<Keys::W>();
      (void) MetaDataOf<Keys::X>();
      (void) MetaDataOf<Keys::Y>();
      (void) MetaDataOf<Keys::Z>();

      (void) MetaDataOf<Keys::Escape>();
      (void) MetaDataOf<Keys::Enter>();
      (void) MetaDataOf<Keys::Tab>();
      (void) MetaDataOf<Keys::Back>();
      (void) MetaDataOf<Keys::Insert>();
      (void) MetaDataOf<Keys::Delete>();
      (void) MetaDataOf<Keys::PageUp>();
      (void) MetaDataOf<Keys::PageDown>();
      (void) MetaDataOf<Keys::Home>();
      (void) MetaDataOf<Keys::End>();

      (void) MetaDataOf<Keys::Left>();
      (void) MetaDataOf<Keys::Right>();
      (void) MetaDataOf<Keys::Up>();
      (void) MetaDataOf<Keys::Down>();

      (void) MetaDataOf<Keys::CapsLock>();
      (void) MetaDataOf<Keys::ScrollLock>();
      (void) MetaDataOf<Keys::NumLock>();
      (void) MetaDataOf<Keys::Print>();
      (void) MetaDataOf<Keys::Pause>();
             
      (void) MetaDataOf<Keys::F1>();
      (void) MetaDataOf<Keys::F2>();
      (void) MetaDataOf<Keys::F3>();
      (void) MetaDataOf<Keys::F4>();
      (void) MetaDataOf<Keys::F5>();
      (void) MetaDataOf<Keys::F6>();
      (void) MetaDataOf<Keys::F7>();
      (void) MetaDataOf<Keys::F8>();
      (void) MetaDataOf<Keys::F9>();
      (void) MetaDataOf<Keys::F10>();
      (void) MetaDataOf<Keys::F11>();
      (void) MetaDataOf<Keys::F12>();
      (void) MetaDataOf<Keys::F13>();
      (void) MetaDataOf<Keys::F14>();
      (void) MetaDataOf<Keys::F15>();
      (void) MetaDataOf<Keys::F16>();
      (void) MetaDataOf<Keys::F17>();
      (void) MetaDataOf<Keys::F18>();
      (void) MetaDataOf<Keys::F19>();
      (void) MetaDataOf<Keys::F20>();
      (void) MetaDataOf<Keys::F21>();
      (void) MetaDataOf<Keys::F22>();
      (void) MetaDataOf<Keys::F23>();
      (void) MetaDataOf<Keys::F24>();

      (void) MetaDataOf<Keys::Numpad0>();
      (void) MetaDataOf<Keys::Numpad1>();
      (void) MetaDataOf<Keys::Numpad2>();
      (void) MetaDataOf<Keys::Numpad3>();
      (void) MetaDataOf<Keys::Numpad4>();
      (void) MetaDataOf<Keys::Numpad5>();
      (void) MetaDataOf<Keys::Numpad6>();
      (void) MetaDataOf<Keys::Numpad7>();
      (void) MetaDataOf<Keys::Numpad8>();
      (void) MetaDataOf<Keys::Numpad9>();

      (void) MetaDataOf<Keys::NumpadEqual>();
      (void) MetaDataOf<Keys::NumpadDecimal>();
      (void) MetaDataOf<Keys::NumpadDivide>();
      (void) MetaDataOf<Keys::NumpadMultiply>();
      (void) MetaDataOf<Keys::NumpadSubtract>();
      (void) MetaDataOf<Keys::NumpadAdd>();
      (void) MetaDataOf<Keys::NumpadEnter>();

      (void) MetaDataOf<Keys::LeftShift>();
      (void) MetaDataOf<Keys::RightShift>();

      (void) MetaDataOf<Keys::LeftControl>();
      (void) MetaDataOf<Keys::RightControl>();

      (void) MetaDataOf<Keys::LeftAlt>();
      (void) MetaDataOf<Keys::RightAlt>();

      (void) MetaDataOf<Keys::LeftMouse>();
      (void) MetaDataOf<Keys::MiddleMouse>();
      (void) MetaDataOf<Keys::RightMouse>();
      (void) MetaDataOf<Keys::Mouse4>();
      (void) MetaDataOf<Keys::Mouse5>();
      (void) MetaDataOf<Keys::Mouse6>();
      (void) MetaDataOf<Keys::Mouse7>();
      (void) MetaDataOf<Keys::Mouse8>();

      (void) MetaDataOf<Keys::Joystick1>();
      (void) MetaDataOf<Keys::Joystick2>();
      (void) MetaDataOf<Keys::Joystick3>();
      (void) MetaDataOf<Keys::Joystick4>();
      (void) MetaDataOf<Keys::Joystick5>();
      (void) MetaDataOf<Keys::Joystick6>();
      (void) MetaDataOf<Keys::Joystick7>();
      (void) MetaDataOf<Keys::Joystick8>();
      (void) MetaDataOf<Keys::Joystick9>();
      (void) MetaDataOf<Keys::Joystick10>();
      (void) MetaDataOf<Keys::Joystick11>();
      (void) MetaDataOf<Keys::Joystick12>();
      (void) MetaDataOf<Keys::Joystick13>();
      (void) MetaDataOf<Keys::Joystick14>();
      (void) MetaDataOf<Keys::Joystick15>();
      (void) MetaDataOf<Keys::Joystick16>();
   }
}

/// MARK: Events                                                              
LANGULUS_DEFINE_EVENT(WindowClose)
LANGULUS_DEFINE_EVENT(WindowFileDrop)
LANGULUS_DEFINE_EVENT(WindowMouseHoverIn)
LANGULUS_DEFINE_EVENT(WindowMouseHoverOut)
LANGULUS_DEFINE_EVENT(WindowResolutionChange)
LANGULUS_DEFINE_EVENT(WindowMinimize)
LANGULUS_DEFINE_EVENT(WindowMaximize)
LANGULUS_DEFINE_EVENT(WindowFocus)
LANGULUS_DEFINE_EVENT(WindowUnfocus)
LANGULUS_DEFINE_EVENT(WindowResize)
LANGULUS_DEFINE_EVENT(WindowMove)
LANGULUS_DEFINE_EVENT(WindowText)

LANGULUS_DEFINE_EVENT(MouseMove)
LANGULUS_DEFINE_EVENT(MouseScroll)

/// MARK: Keys                                                                
LANGULUS_DEFINE_KEY(Space)
LANGULUS_DEFINE_KEY(Apostrophe)
LANGULUS_DEFINE_KEY(Comma)
LANGULUS_DEFINE_KEY(Minus)
LANGULUS_DEFINE_KEY(Period)
LANGULUS_DEFINE_KEY(Slash)
LANGULUS_DEFINE_KEY(Hack)
LANGULUS_DEFINE_KEY(Semicolon)
LANGULUS_DEFINE_KEY(LeftBracket)
LANGULUS_DEFINE_KEY(RightBracket)
LANGULUS_DEFINE_KEY(Tilde)

LANGULUS_DEFINE_KEY(Main0)
LANGULUS_DEFINE_KEY(Main1)
LANGULUS_DEFINE_KEY(Main2)
LANGULUS_DEFINE_KEY(Main3)
LANGULUS_DEFINE_KEY(Main4)
LANGULUS_DEFINE_KEY(Main5)
LANGULUS_DEFINE_KEY(Main6)
LANGULUS_DEFINE_KEY(Main7)
LANGULUS_DEFINE_KEY(Main8)
LANGULUS_DEFINE_KEY(Main9)

LANGULUS_DEFINE_KEY(A)
LANGULUS_DEFINE_KEY(B)
LANGULUS_DEFINE_KEY(C)
LANGULUS_DEFINE_KEY(D)
LANGULUS_DEFINE_KEY(E)
LANGULUS_DEFINE_KEY(F)
LANGULUS_DEFINE_KEY(G)
LANGULUS_DEFINE_KEY(H)
LANGULUS_DEFINE_KEY(I)
LANGULUS_DEFINE_KEY(J)
LANGULUS_DEFINE_KEY(K)
LANGULUS_DEFINE_KEY(L)
LANGULUS_DEFINE_KEY(M)
LANGULUS_DEFINE_KEY(N)
LANGULUS_DEFINE_KEY(O)
LANGULUS_DEFINE_KEY(P)
LANGULUS_DEFINE_KEY(Q)
LANGULUS_DEFINE_KEY(R)
LANGULUS_DEFINE_KEY(S)
LANGULUS_DEFINE_KEY(T)
LANGULUS_DEFINE_KEY(U)
LANGULUS_DEFINE_KEY(V)
LANGULUS_DEFINE_KEY(W)
LANGULUS_DEFINE_KEY(X)
LANGULUS_DEFINE_KEY(Y)
LANGULUS_DEFINE_KEY(Z)

LANGULUS_DEFINE_KEY(Escape)
LANGULUS_DEFINE_KEY(Enter)
LANGULUS_DEFINE_KEY(Tab)
LANGULUS_DEFINE_KEY(Back)
LANGULUS_DEFINE_KEY(Insert)
LANGULUS_DEFINE_KEY(Delete)
LANGULUS_DEFINE_KEY(PageUp)
LANGULUS_DEFINE_KEY(PageDown)
LANGULUS_DEFINE_KEY(Home)
LANGULUS_DEFINE_KEY(End)

LANGULUS_DEFINE_KEY(Left)
LANGULUS_DEFINE_KEY(Right)
LANGULUS_DEFINE_KEY(Up)
LANGULUS_DEFINE_KEY(Down)

LANGULUS_DEFINE_KEY(CapsLock)
LANGULUS_DEFINE_KEY(ScrollLock)
LANGULUS_DEFINE_KEY(NumLock)
LANGULUS_DEFINE_KEY(Print)
LANGULUS_DEFINE_KEY(Pause)

LANGULUS_DEFINE_KEY(F1)
LANGULUS_DEFINE_KEY(F2)
LANGULUS_DEFINE_KEY(F3)
LANGULUS_DEFINE_KEY(F4)
LANGULUS_DEFINE_KEY(F5)
LANGULUS_DEFINE_KEY(F6)
LANGULUS_DEFINE_KEY(F7)
LANGULUS_DEFINE_KEY(F8)
LANGULUS_DEFINE_KEY(F9)
LANGULUS_DEFINE_KEY(F10)
LANGULUS_DEFINE_KEY(F11)
LANGULUS_DEFINE_KEY(F12)
LANGULUS_DEFINE_KEY(F13)
LANGULUS_DEFINE_KEY(F14)
LANGULUS_DEFINE_KEY(F15)
LANGULUS_DEFINE_KEY(F16)
LANGULUS_DEFINE_KEY(F17)
LANGULUS_DEFINE_KEY(F18)
LANGULUS_DEFINE_KEY(F19)
LANGULUS_DEFINE_KEY(F20)
LANGULUS_DEFINE_KEY(F21)
LANGULUS_DEFINE_KEY(F22)
LANGULUS_DEFINE_KEY(F23)
LANGULUS_DEFINE_KEY(F24)

LANGULUS_DEFINE_KEY(Numpad0)
LANGULUS_DEFINE_KEY(Numpad1)
LANGULUS_DEFINE_KEY(Numpad2)
LANGULUS_DEFINE_KEY(Numpad3)
LANGULUS_DEFINE_KEY(Numpad4)
LANGULUS_DEFINE_KEY(Numpad5)
LANGULUS_DEFINE_KEY(Numpad6)
LANGULUS_DEFINE_KEY(Numpad7)
LANGULUS_DEFINE_KEY(Numpad8)
LANGULUS_DEFINE_KEY(Numpad9)

LANGULUS_DEFINE_KEY(NumpadEqual)
LANGULUS_DEFINE_KEY(NumpadDecimal)
LANGULUS_DEFINE_KEY(NumpadDivide)
LANGULUS_DEFINE_KEY(NumpadMultiply)
LANGULUS_DEFINE_KEY(NumpadSubtract)
LANGULUS_DEFINE_KEY(NumpadAdd)
LANGULUS_DEFINE_KEY(NumpadEnter)

LANGULUS_DEFINE_KEY(LeftShift)
LANGULUS_DEFINE_KEY(RightShift)

LANGULUS_DEFINE_KEY(LeftControl)
LANGULUS_DEFINE_KEY(RightControl)

LANGULUS_DEFINE_KEY(LeftAlt)
LANGULUS_DEFINE_KEY(RightAlt)

LANGULUS_DEFINE_KEY(LeftGUI)
LANGULUS_DEFINE_KEY(RightGUI)

LANGULUS_DEFINE_KEY(LeftMouse)
LANGULUS_DEFINE_KEY(MiddleMouse)
LANGULUS_DEFINE_KEY(RightMouse)
LANGULUS_DEFINE_KEY(Mouse4)
LANGULUS_DEFINE_KEY(Mouse5)
LANGULUS_DEFINE_KEY(Mouse6)
LANGULUS_DEFINE_KEY(Mouse7)
LANGULUS_DEFINE_KEY(Mouse8)

LANGULUS_DEFINE_KEY(Joystick1)
LANGULUS_DEFINE_KEY(Joystick2)
LANGULUS_DEFINE_KEY(Joystick3)
LANGULUS_DEFINE_KEY(Joystick4)
LANGULUS_DEFINE_KEY(Joystick5)
LANGULUS_DEFINE_KEY(Joystick6)
LANGULUS_DEFINE_KEY(Joystick7)
LANGULUS_DEFINE_KEY(Joystick8)
LANGULUS_DEFINE_KEY(Joystick9)
LANGULUS_DEFINE_KEY(Joystick10)
LANGULUS_DEFINE_KEY(Joystick11)
LANGULUS_DEFINE_KEY(Joystick12)
LANGULUS_DEFINE_KEY(Joystick13)
LANGULUS_DEFINE_KEY(Joystick14)
LANGULUS_DEFINE_KEY(Joystick15)
LANGULUS_DEFINE_KEY(Joystick16)
