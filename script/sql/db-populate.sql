/*--------------------------INSERT DATA---------------------------------------*/

/*TABLE USERS*/

INSERT INTO users(NAME,PASS,INFORMATION) VALUES('Pepe','contrasena1','Guapo y mas que guapo');
INSERT INTO users(NAME,PASS,INFORMATION) VALUES('Antonio','contrasena2','Feo');
INSERT INTO users(NAME,PASS,INFORMATION) VALUES('Maria','contrasena3','Gorda');
INSERT INTO users(NAME,PASS,INFORMATION) VALUES('Carmen','contrasena4','Delgada');
INSERT INTO users(NAME,PASS,INFORMATION) VALUES('Carlos','contrasena5','Indiferente');

/*TABLE CHATS*/

INSERT INTO chats(ID_ADMIN,DESCRIPTION,CREATION_TIME) VALUES(1,'chat 1',0);
INSERT INTO chats(ID_ADMIN,DESCRIPTION,CREATION_TIME) VALUES(1,'chat 2',0);
INSERT INTO chats(ID_ADMIN,DESCRIPTION,CREATION_TIME) VALUES(2,'chat 3',0);
INSERT INTO chats(ID_ADMIN,DESCRIPTION,CREATION_TIME) VALUES(4,'chat 4',0);

/*TABLE USER_CHATS*/

INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(1,1,0,0);
INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(1,2,0,0);
INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(2,3,0,0);
INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(3,1,0,0);
INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(3,2,0,0);
INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(3,3,0,0);
INSERT INTO users_chats(ID_USERS,ID_CHAT,CREATION_TIME,READ_MSG_TIME) VALUES(4,3,0,0);

/*TABLE FRIENS*/

INSERT INTO friends(ID1,ID2,CREATION_TIME) VALUES(1,2,0);
INSERT INTO friends(ID1,ID2,CREATION_TIME) VALUES(3,2,0);
INSERT INTO friends(ID1,ID2,CREATION_TIME) VALUES(3,4,0);
INSERT INTO friends(ID1,ID2,CREATION_TIME) VALUES(2,4,0);

/*TABLE REQUEST*/

INSERT INTO friends_request(ID1,ID2_request,CREATION_TIME) VALUES(1,3,0);
INSERT INTO friends_request(ID1,ID2_request,CREATION_TIME) VALUES(3,5,0);
INSERT INTO friends_request(ID1,ID2_request,CREATION_TIME) VALUES(5,2,0);

/*TABLE MESSAGES*/

INSERT INTO messages(ID_SENDER,ID_CHAT,FILE_,TEXT,CREATION_TIME) VALUES(3,3,'fichero.txt','hola que tal guapo?',20);
INSERT INTO messages(ID_SENDER,ID_CHAT,FILE_,TEXT,CREATION_TIME) VALUES(4,3,'null','pues muy bien y tsdfu?',22);
INSERT INTO messages(ID_SENDER,ID_CHAT,FILE_,TEXT,CREATION_TIME) VALUES(4,3,'null','tuu?*',20);
INSERT INTO messages(ID_SENDER,ID_CHAT,FILE_,TEXT,CREATION_TIME) VALUES(1,1,'null','uuuuooola',15);
INSERT INTO messages(ID_SENDER,ID_CHAT,FILE_,TEXT,CREATION_TIME) VALUES(1,1,'null','uuuuooola*2',25);
INSERT INTO messages(ID_SENDER,ID_CHAT,FILE_,TEXT,CREATION_TIME) VALUES(2,4,'null','uuuuooola*2',25);

