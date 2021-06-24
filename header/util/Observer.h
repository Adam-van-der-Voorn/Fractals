#pragma once
template <class T>
class Observer
   {
   public:
      virtual ~Observer() {}
      virtual void onNotify(T *subject, int event_num)= 0;
   };
