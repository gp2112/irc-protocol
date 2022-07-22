#ifndef ERRORS_H
#define ERRORS_H



// Channel errors are bytes in 001BBBBBB

#define ERR_NEEDMOREPARAMS     461    
#define ERR_BANNEDFROMCHAN     474
#define ERR_INVITEONLYCHAN     473         
#define ERR_CHANNELISFULL      471
#define ERR_BADCHANNELKEY      475          
#define ERR_BADCHANMASK        476
#define ERR_NOSUCHNICK         401
#define ERR_NOSUCHCHANNEL      403             
#define ERR_TOOMANYCHANNELS    405
#define RPL_TOPIC              332
#define ERR_NOTONCHANNEL       442
#define ERR_USERNOTINCHANNEL   441
#define ERR_CHANOPRIVSNEEDED   482
#define ERR_TOOMANYCHANNELS    405
#define ERR_YOUREBANNEDCREEP   465
#define ERR_ALREADYREGISTRED   462
#define ERR_NOTREGISTERED      451
#define ERR_NOSUCHSERVER       402
#define ERR_NOTEXTTOSEND       412
#define ERR_UNKNOWNCOMMAND     421
#define ERR_NOPRIVILEGES       481

#define ERR_NEEDMOREPARAMS_SRT  "Command need more parameters"
#define ERR_BANNEDFROMCHAN_SRT  "You are banned here"
#define ERR_INVITEONLYCHAN_SRT  "This channel is acessible with invites"
#define ERR_CHANNELISFULL_SRT   "This channel is already Full"
#define ERR_BADCHANNELKEY_SRT   "Wrong channel Password"
#define ERR_BADCHANMASK_SRT     ""
#define ERR_NOSUCHNICK_SRT      "You need a nick before try this"
#define ERR_NOSUCHCHANNEL_SRT   "This channel doesnt exists"
#define ERR_TOOMANYCHANNELS_SRT "Error: There are to manny channels"
#define RPL_TOPIC_SRT           ""
#define ERR_NOTONCHANNEL_SRT    ""
#define ERR_USERNOTINCHANNEL_SRT "This user is not in the channel"
#define ERR_CHANOPRIVSNEEDED_SRT ""
#define ERR_YOUREBANNEDCREEP_SRT "YOU ARE BANNED AGAIN"
#define ERR_ALREADYREGISTRED_SRT "Already registred"
#define ERR_NOTREGISTERED_SRT    "You need to register first"
#define ERR_NOSUCHSERVER_SRT     "Server not rechable"
#define ERR_NOTEXTTOSEND_SRT     "No text to send"
#define ERR_UNKNOWNCOMMAND_SRT   "This command doesnt exists"
#define ERR_NOPRIVILEGES_SRT     "You dont have privileges to do it"

#endif
