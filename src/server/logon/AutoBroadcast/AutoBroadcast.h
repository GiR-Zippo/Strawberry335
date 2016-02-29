#ifndef __AUTO_BROADCAST
#define __AUTO_BROADCAST

class AutoBroadcast
{
public:
    static AutoBroadcast& instance() {
        static AutoBroadcast auto_broadcast;

        return auto_broadcast;
    }

    void load();

    void send();

private:
    std::vector<std::string> m_Autobroadcasts;
};

#define sAutoBroadcast AutoBroadcast::instance()

#endif /* __AUTO_BROADCAST */
