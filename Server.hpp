#pragma once
#ifndef TERMINAL_SERVER_HPP
#define TERMINAL_SERVER_HPP

/* Terminal: Server
   #include "Server.hpp"
   
   The server tracks its internal state, and sends and recieves packets.
   Mostly used for text-only websites and also databases.
   
   PACKET TYPES:
   
   PIN - Ping. Ping target.
   CON - Connect. Request connection.
   ACK - Acknowledge. Acknowledge connection.
   RPG - Request page. Request current page data from server.
   REJ - Reject. Reject connection.
   DIS - Disconnect. Disconnect connection.
   PGD - Page data. Data to render requested page. Entire page is passed at once for simplicity.
   
   RDR - Redraw. Client should request new page data.
   
*/

#include <string>

class Server
{
   public:
   
   std::string number;
   
   Server()
   {
   }
   
   // Server will return the main page upon successful connection.
   virtual std::string servePage()
   {
      std::string mainPkt = "[PGD][data]";
      return "";
   }
   
   virtual std::string recievePacket(std::string _packet)
   {
      return "";
   }
   virtual void sendPacket(std::string _packet)
   {
   }
};

Server testServer;



/*
   RedThread is public BBS. Browse threads with up/down arrow.

*/

class Server_RedThread: public Server
{
   
   class Thread
   {
      public:
      
      std::string title;
      std::string mainText;

      Vector <std::string> vReply;
      
      Thread(std::string _title="")
      {
         title=_title;
         mainText="";
      }
      void addReply(std::string _reply)
      {
         vReply.push(_reply);
      }
   };
   
   public:
   
   //std::string number;
   std::string userHandle;
   
   Vector <Thread*> vThread;
   int currentThread; // size() means new thread.
   
   
   enum ActiveMenu
   {
      MAIN,
      BBS,
         READ_THREAD,
         REPLY_THREAD
   };
    ActiveMenu activeMenu;
   
   Server_RedThread()
   {
      number = "0011111111";
      userHandle="";
      
      activeMenu=MAIN;
      currentThread = 0;
      
      initThreads();
   }
   
   void initThreads()
   {
      char r = 1;
      std::string redacted  = std::string() + r + "" + r + "/" + r + r + "/" +r +r +r +r;
      
      Thread * thread1 = new Thread("TEST");
      thread1->mainText = "AYY THIS IS A TEST OF THE BBS THREAD SYSTEM.";
      vThread.push(thread1);
      
      Thread * thread2 = new Thread("HOW DO I POST BBS");
      thread2->mainText = "AYO THIS IS ALSO A TEST OF THE BBS THREAD SYSTEM.";
      thread2->addReply("Hey boi I heard you were testing the BBS thread system.");
      thread2->addReply("Damn straight son.");
      vThread.push(thread2);
      
      Thread * thread3 = new Thread("HELP I AM STUCK IN COMPUTER WORLD");
      thread3->mainText = "FUCK THIS BULLSHIT ISEKAI I DON'T EVEN HAVE A HAREM.";
      thread3->addReply("lol sux 2b u.");
      vThread.push(thread3);
   }
   
   virtual std::string servePage()
   {
      std::string strMain = "";
      if (activeMenu==MAIN)
      {
         strMain+="REDTHREAD\n\033[1;31m________________________________________________________________\033[0m";
         strMain+= "\n\nPlease enter a handle:";
      }
      else if (activeMenu==BBS)
      {
         strMain+="TOPIC LIST\n\033[1;31m________________________________________________________________\033[0m";
         
         //int currentY=2;
         
         for (int i=0;i<vThread.size();++i)
         {
            strMain+=DataTools::toString(i);
            strMain+=". ";
            strMain+=vThread(i)->title;
            strMain+="\n";
         }
         strMain+="\n#, new, or exit.\n";
      }
      else if (activeMenu==READ_THREAD)
      {
         if ( vThread.isSafe(currentThread) )
         {
            Thread* t = vThread(currentThread);
            
            strMain+="READING THREAD: "+t->title+"\n\033[1;31m________________________________________________________________\033[0m";
            strMain+=t->mainText+"\n\033[1;31m________________________________________________________________\033[0m";
            
            for (int i2=0;i2<t->vReply.size();++i2)
            {
               strMain+=t->vReply(i2)+"\n\033[1;31m________________________________________________________________\033[0m";
            }
            //put cursor bottom.
            strMain+="\n\nreply or exit\n";
         }
      }
      else if (activeMenu==REPLY_THREAD)
      {
         strMain="Write reply (enter to send):\n";
      }


      return strMain;
   }
   
   // To scroll through BBS topics or posts.
   void scrollDown()
   {
   }
   void scrollUp()
   {
   }
   

   std::string recievePacket(std::string _command)
   {
      if ( activeMenu==MAIN )
      {
         activeMenu=BBS;
         // send redraw command
         return "[RDR]";
      }
      else if (activeMenu==BBS)
      {
         if (_command=="EXIT")
         {
            activeMenu=MAIN;
         }
         else if (DataTools::isNumber(_command))
         {
            std::cout<<"READ TOPIC: "<<_command<<".\n";
            
            int topicNumber = DataTools::toInt(_command);
            
            if ( topicNumber < vThread.size() )
            {
               activeMenu=READ_THREAD;
               currentThread=topicNumber;
               
            }
         }
         
         //activeMenu=MAIN;
         // send redraw command
         return "[RDR]";
      }
      else if (activeMenu==READ_THREAD)
      {
         if (_command=="EXIT")
         {
            activeMenu=BBS;
            return "[RDR]";
         }
         else if (_command=="REPLY")
         {
            activeMenu=REPLY_THREAD;
            return "[RDR]";
         }
      }
      else if (activeMenu==REPLY_THREAD)
      {
         if (vThread.isSafe(currentThread))
         {
            vThread(currentThread)->addReply(_command);
            activeMenu=READ_THREAD;
            return "[RDR]";
         }
      }
      
      return "";
   }
};
Server_RedThread serverRedThread;

// Mail can be stored using real encryption, which will require password to read.
class Server_SpicyMail: public Server
{
   public:
   
   enum ActiveMenu
   {
      LOGIN,
      MAIL_USER,
         MAIL_ADMIN
   };
    ActiveMenu activeMenu;
    
    std::string username;
    std::string password;
   
   Server_SpicyMail()
   {
      number = "0012222222";
      activeMenu=LOGIN;
      username="";
      password="";
   }
   
   virtual std::string servePage()
   {
      std::string strMain = "";
      if (activeMenu==LOGIN)
      {
         strMain+="WELCOME TO SPICYMAIL";
         strMain+= "\nUsername:\nPassword:\033[1,1H"; /* put cursor at username */
      }
      else if (activeMenu==MAIL_USER)
      {
         strMain+="MAIL\nYou have x unread messages.";
      }
      return strMain;
   }
};
Server_SpicyMail serverMail;

class Server_LumaBank: public Server
{
   public:
   
   enum ActiveMenu
   {
      LOGIN,
      CASHMONEY
   };
    ActiveMenu activeMenu;
   
   Server_LumaBank()
   {
      number="0013333333";
   }
};
Server_LumaBank serverLumaBank;


#endif
