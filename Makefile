CC = g++
CFLAGS = -g -fPIC -D_FILELINE -Wno-deprecated 

#HOME = /mnt/Shared/T20150901
HOME = /usr/games/login_V20150901

MYSQLINC = /usr/include/mysql
MYSQLLIB = -L /usr/lib64/mysql -l mysqlclient -lz -lm

INC = -I$(HOME)/Utility \
	  -I$(HOME)/Network  \
	  -I$(HOME)/Public \
	  -I$(HOME)/Common \
	  -I$(HOME)/LoginSrv \
	  -I$(HOME)/JsonSrv \
	  -I$(HOME)/DBSrv \
	  -I $(HOME)/HyMysql \
	  -I$(MYSQLINC) 
	  

OBJS = 	Utility/Yond_mutex.o \
	    Utility/Yond_thread.o \
		Utility/Yond_thread_pool.o \
		Utility/Yond_ini.o \
		Network/Acceptor.o \
		Network/Connector.o \
		Network/Session.o \
		Network/SessionList.o \
		Network/SessionPool.o \
		Network/SyncHandler.o \
		Network/SocketOpt.o \
		Network/NetworkObject.o \
		Network/IOCPServer.o \
		Public/InfoParser.o \
		Public/Yond_json.o \
		Public/JsonParser.o \
		Public/MsgBuff.o \
		Public/Yond_drng.o \
		Public/Yond_md5.o 
		
JSONS =	JsonSrv/JsonFactory.o \
		JsonSrv/JsonServer.o \
		JsonSrv/ServerSession.o \
		JsonSrv/PacketHandler.o \
		JsonSrv/Handler_FromDBServer.o \
		JsonSrv/Handler_FromLoginServer.o \
		JsonSrv/LoginServerSession.o \
		JsonSrv/DBServerSession.o \
		JsonSrv/Json_PreLoginREQ.o \
		JsonSrv/Json_PreLoginANC.o \
		JsonSrv/JsonMain.o

Login =	LoginSrv/BinarySession.o \
		LoginSrv/HttpSession.o \
		LoginSrv/JsonSession.o \
		LoginSrv/KeyvSession.o \
		LoginSrv/LoginFactory.o \
		LoginSrv/ServerSession.o \
		LoginSrv/TempServerSession.o \
		LoginSrv/UserSession.o \
		LoginSrv/LoginServer.o \
		LoginSrv/LoginMain.o
		
DBSrv =	HyMysql/HyDatabase.o \
		HyMysql/IDBCInterface.o \
		HyMysql/IMysql.o \
		HyMysql/QueryExecuteProcessor.o \
		HyMysql/QueryResult.o \
		DBSrv/DBConnectDir.o \
		DBSrv/DBFactory.o \
		DBSrv/DBMain.o \
		DBSrv/DBServer.o \
		DBSrv/Handler_FromJsonServer.o \
		DBSrv/JsonServerSession.o \
		DBSrv/PacketHandler.o \
		DBSrv/ServerSession.o \
		DBSrv/TempServerSession.o \
		DBSrv/PreLoginQuery.o
		
BINDIR = $(HOME)/bin

all: checkbin $(BINDIR)/JsonSrv $(BINDIR)/LoginSrv $(BINDIR)/DBSrv 

$(BINDIR)/JsonSrv: $(JSONS) $(OBJS)
	$(CC) -g $^ -o $@ -pthread

$(BINDIR)/LoginSrv: $(Login) $(OBJS)
	$(CC) -g $^ -o $@ -pthread
	
	
$(BINDIR)/DBSrv: $(DBSrv) $(OBJS)
	$(CC) -g $(MYSQLLIB) $^ -o $@ -pthread
	
	
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

.PHONY: checkbin
checkbin:
	test -d "${BINDIR}" || mkdir $(BINDIR)

.PHONY:	clean
clean:
	rm -f ./*.o
	rm -f Utility/*.o
	rm -f Network/*.o
	rm -f Public/*.o
	
	rm -f LoginSrv/*.o
	rm -f $(BINDIR)/LoginSrv
	
cleanDB:
	rm -f DBSrv/*.o
	rm -f HyMysql/*.o
	rm -f $(BINDIR)/DBSrv
	
cleanJ:
	rm -f JsonSrv/*.o
	rm -f $(BINDIR)/JsonSrv
	
