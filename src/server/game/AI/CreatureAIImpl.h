#ifndef CREATUREAIIMPL_H
#define CREATUREAIIMPL_H

#include "Common.h"
#include "Define.h"
#include "TemporarySummon.h"
#include "CreatureAI.h"
#include "SpellMgr.h"

template<class T>
inline
const T& RAND(const T& v1, const T& v2)
{
    return (urand(0, 1)) ? v1 : v2;
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3)
{
    switch (urand(0, 2))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4)
{
    switch (urand(0, 3))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5)
{
    switch (urand(0, 4))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6)
{
    switch (urand(0, 5))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7)
{
    switch (urand(0, 6))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8)
{
    switch (urand(0, 7))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9)
{
    switch (urand(0, 8))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10)
{
    switch (urand(0, 9))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10, const T& v11)
{
    switch (urand(0, 10))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
        case 10: return v11;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10, const T& v11, const T& v12)
{
    switch (urand(0, 11))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
        case 10: return v11;
        case 11: return v12;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10, const T& v11, const T& v12, const T& v13)
{
    switch (urand(0, 12))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
        case 10: return v11;
        case 11: return v12;
        case 12: return v13;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10, const T& v11, const T& v12, const T& v13, const T& v14)
{
    switch (urand(0, 13))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
        case 10: return v11;
        case 11: return v12;
        case 12: return v13;
        case 13: return v14;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10, const T& v11, const T& v12, const T& v13, const T& v14, const T& v15)
{
    switch (urand(0, 14))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
        case 10: return v11;
        case 11: return v12;
        case 12: return v13;
        case 13: return v14;
        case 14: return v15;
    }
}

template<class T>
inline
const T& RAND(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8,
              const T& v9, const T& v10, const T& v11, const T& v12, const T& v13, const T& v14, const T& v15, const T& v16)
{
    switch (urand(0, 15))
    {
        default:
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        case 5: return v6;
        case 6: return v7;
        case 7: return v8;
        case 8: return v9;
        case 9: return v10;
        case 10: return v11;
        case 11: return v12;
        case 12: return v13;
        case 13: return v14;
        case 14: return v15;
        case 15: return v16;
    }
}

class EventMap
{
    typedef std::multimap<uint32, uint32> EventStore;
    
public:
    EventMap() : _time(0), _phase(0) {}
    
    
    // Removes all events and clears phase
    void Reset()
    {
        _eventMap.clear();
        _time = 0;
        _phase = 0;
    }
    
    void Update(uint32 time)
    {
        _time += time;
    }
    // Returns current timer value, does not represent real dates/times
    uint32 GetTimer() const
    {
        return _time;
    }
    
    uint32 GetPhaseMask() const
    {
        return _phase;
    }
    
    // returns true if eventId is in list
    bool IsEventActive(uint32 eventId)
    {
        if (Empty())
            return false;

        for (EventStore::const_iterator itr = _eventMap.begin(); itr != _eventMap.end(); ++itr)
            if ((itr->second & 0x0000FFFF) == eventId)
                return true;

        return false;
    }

    bool Empty() const
    {
        return _eventMap.empty();
    }
    
    // Sets event phase, must be in range 1 - 8
    void SetPhase(uint32 phase)
    {
        if (!phase)
            _phase = 0;
        else if (phase <= 8)
            _phase = (1 << (phase - 1));
    }
    
    // Activates event phase, must be in range 1 - 8
    void AddPhase(uint32 phase)
    {
        if (phase && phase <= 8)
            _phase |= (1 << (phase - 1));
    }
    
    // Deactivates event phase, must be in range 1 - 8
    void RemovePhase(uint32 phase)
    {
        if (phase && phase <= 8)
            _phase &= ~(1 << (phase - 1));
    }
    
    // Creates new event entry in map with given id, time, group if given (1 - 8) and phase if given (1 - 8)
    // 0 for group/phase means it belongs to no group or runs in all phases
    void ScheduleEvent(uint32 eventId, uint32 time, uint32 group = 0, uint32 phase = 0)
    {
        if (group && group <= 8)
            eventId |= (1 << (group + 15));
        
        if (phase && phase <= 8)
            eventId |= (1 << (phase + 23));
        
        _eventMap.insert(EventStore::value_type(_time + time, eventId));
        
    }
    
    // Removes event with specified id and creates new entry for it
    void RescheduleEvent(uint32 eventId, uint32 time, uint32 group = 0, uint32 phase = 0)
    {
        CancelEvent(eventId);
        ScheduleEvent(eventId, time, group, phase);
    }
    
    // Reschedules closest event
    void RepeatEvent(uint32 time)
    {
        if (Empty())
            return;
        
        uint32 eventId = _eventMap.begin()->second;
        _eventMap.erase(_eventMap.begin());
        
        ScheduleEvent(eventId, time);
    }
    
    // Removes first event
    void PopEvent()
    {
        if (!Empty())
            _eventMap.erase(_eventMap.begin());
    }
    
    // Gets next event id to execute and removes it from map
    uint32 ExecuteEvent()
    {
        while (!Empty())
        {
            EventStore::iterator itr = _eventMap.begin();
            
            if (itr->first > _time)
                return 0;
            else if (_phase && (itr->second & 0xFF000000) && !((itr->second >> 24) & _phase))
                _eventMap.erase(itr);
            else
            {
                uint32 eventId = (itr->second & 0x0000FFFF);
                _eventMap.erase(itr);
                return eventId;
            }
        }
        
        return 0;
    }
    
    // Gets next event id to execute
    uint32 GetEvent()
    {
        while (!Empty())
        {
            EventStore::iterator itr = _eventMap.begin();
            if (itr->first > _time)
                return 0;
            else if (_phase && (itr->second & 0xFF000000) && !(itr->second & (_phase << 24)))
                _eventMap.erase(itr);
            else
                return (itr->second & 0x0000FFFF);
        }
        
        return 0;
    }
    
    // Delay all events
    void DelayEvents(uint32 delay)
    {
        _time = delay < _time ? _time - delay : 0;
    }
    
    // Delay all events having the specified Group
    void DelayEvents(uint32 delay, uint32 group)
    {
        if (!group || group > 8 || Empty())
            return;
        
        EventStore delayed;
        
        for (EventStore::iterator itr = _eventMap.begin(); itr != _eventMap.end();)
        {
            if (itr->second & (1 << (group + 15)))
            {
                delayed.insert(EventStore::value_type(itr->first + delay, itr->second));
                _eventMap.erase(itr++);
            }
            else
                ++itr;
        }
        
        _eventMap.insert(delayed.begin(), delayed.end());
    }
    
    // Cancel events with specified id
    void CancelEvent(uint32 eventId)
    {
        if (Empty())
            return;
        
        for (EventStore::iterator itr = _eventMap.begin(); itr != _eventMap.end();)
        {
            if (eventId == (itr->second & 0x0000FFFF))
                _eventMap.erase(itr++);
            else
                ++itr;
        }
    }
    
    // Cancel events belonging to specified group
    void CancelEventGroup(uint32 group)
    {
        if (!group || group > 8 || Empty())
            return;
        
        for (EventStore::iterator itr = _eventMap.begin(); itr != _eventMap.end();)
        {
            if (itr->second & (1 << (group + 15)))
                _eventMap.erase(itr++);
            else
                ++itr;
        }
    }
    
    // Returns time of next event to execute
    // To get how much time remains substract _time
    uint32 GetNextEventTime(uint32 eventId) const
    {
        if (Empty())
            return 0;
        
        for (EventStore::const_iterator itr = _eventMap.begin(); itr != _eventMap.end(); ++itr)
            if (eventId == (itr->second & 0x0000FFFF))
                return itr->first;
            
            return 0;
    }
    
    uint32 GetNextEventTime() const
    {
        return Empty() ? 0 : _eventMap.begin()->first;
    }
    
    // Returns wether the eventmap is in the given phase or not.
    bool IsInPhase(uint32 phase)
    {
        return phase <= 8 && (!phase || _phase & (1 << (phase - 1)));
    }
    
private:
    uint32 _time;
    uint32 _phase;
    
    EventStore _eventMap;
};

enum AITarget
{
    AITARGET_SELF,
    AITARGET_VICTIM,
    AITARGET_ENEMY,
    AITARGET_ALLY,
    AITARGET_BUFF,
    AITARGET_DEBUFF,
};

enum AICondition
{
    AICOND_AGGRO,
    AICOND_COMBAT,
    AICOND_DIE,
};

#define AI_DEFAULT_COOLDOWN 5000

struct AISpellInfoType
{
    AISpellInfoType() : target(AITARGET_SELF), condition(AICOND_COMBAT)
    , cooldown(AI_DEFAULT_COOLDOWN), realCooldown(0), maxRange(0.0f){}
    AITarget target;
    AICondition condition;
    uint32 cooldown;
    uint32 realCooldown;
    float maxRange;
};

AISpellInfoType* GetAISpellInfo(uint32 i);

#endif