/*2.3 Messages

   Servers and clients send eachother messages which may or may not
   generate a reply.  If the message contains a valid command, as
   described in later sections, the client should expect a reply as
   specified but it is not advised to wait forever for the reply; client
   to server and server to server communication is essentially
   asynchronous in nature.

   Each IRC message may consist of up to three main parts: the prefix
   (optional), the command, and the command parameters (of which there
   may be up to 15).  The prefix, command, and all parameters are
   separated by one (or more) ASCII space character(s) (0x20).

   The presence of a prefix is indicated with a single leading ASCII
   colon character (':', 0x3b), which must be the first character of the
   message itself.  There must be no gap (whitespace) between the colon
   and the prefix.  The prefix is used by servers to indicate the true

   origin of the message.  If the prefix is missing from the message, it
   is assumed to have originated from the connection from which it was
   received.  Clients should not use prefix when sending a message from
   themselves; if they use a prefix, the only valid prefix is the
   registered nickname associated with the client.  If the source
   identified by the prefix cannot be found from the server's internal
   database, or if the source is registered from a different link than
   from which the message arrived, the server must ignore the message
   silently.

   The command must either be a valid IRC command or a three (3) digit
   number represented in ASCII text.

   IRC messages are always lines of characters terminated with a CR-LF
   (Carriage Return - Line Feed) pair, and these messages shall not
   exceed 512 characters in length, counting all characters including
   the trailing CR-LF. Thus, there are 510 characters maximum allowed
   for the command and its parameters.  There is no provision for
   continuation message lines.  See section 7 for more details about
   current implementations.

    <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
    <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
    <command>  ::= <letter> { <letter> } | <number> <number> <number>
    <SPACE>    ::= ' ' { ' ' }
    <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]

    <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
                   or NUL or CR or LF, the first of which may not be ':'>
    <trailing> ::= <Any, possibly *empty*, sequence of octets not including
                     NUL or CR or LF>

    <crlf>     ::= CR LF

    2.4 Numeric replies

   Most of the messages sent to the server generate a reply of some
   sort.  The most common reply is the numeric reply, used for both
   errors and normal replies.  The numeric reply must be sent as one
   message consisting of the sender prefix, the three digit numeric, and
   the target of the reply.  A numeric reply is not allowed to originate
   from a client; any such messages received by a server are silently
   dropped. In all other respects, a numeric reply is just like a normal
   message, except that the keyword is made up of 3 numeric digits
   rather than a string of letters.  A list of different replies is
   supplied in section 6.

*/ 

struct msg_ {
    char *prefix;
    char *cmd;
    char *params; // separated by space
};
