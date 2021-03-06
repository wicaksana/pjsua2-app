#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

//subclass to extend the Account and get notifications etc
class MyAccount: public Account{
    public:
        virtual void onRegState(OnRegStateParam &prm){
            AccountInfo ai = getInfo();
            std::cout << (ai.regIsActive? "*** Register:" : "*** Unregister:")
                      << " code=" << prm.code << std::endl;
        }
};

int main()
{
    Endpoint ep;

    ep.libCreate();

    //initialize endpoint
    EpConfig ep_cfg;
    ep.libInit( ep_cfg );

    //create SIP transport. Error handling sample is shown
    TransportConfig tcfg;
    tcfg.port = 5060;
    try{
        ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    } catch(Error &err) {
        std::cout << err.info() << std::endl;
        return 1;
    }

    //start the library (worker threads etc)
    ep.libStart();
    std::cout << "***PJSUA started***" << std::endl;

    //configure an AccountConfig
    AccountConfig acfg;
    acfg.idUri = "sip:test@pjsip.org";
    acfg.regConfig.registrarUri = "sip:pjsip.org";
    AuthCredInfo cred("digest", "*", "test", 0, "secret");
    acfg.sipConfig.authCreds.push_back( cred );

    //create the account
    MyAccount *acc = new MyAccount;
    acc->create(acfg);

    //here we don't have anything else to do..
    pj_thread_sleep(10000);

    //delete the account. this will unregister from server
    delete acc;

    //this will implicitly shutdown the library
    return 0;

}
