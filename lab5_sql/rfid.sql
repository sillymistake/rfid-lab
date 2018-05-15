drop database if exists `rfid`;
create database `rfid` character set utf8;
use `rfid`;

CREATE TABLE `card` (
`money`  varchar(255) NOT NULL ,
`uid`  varchar(8) NOT NULL ,
PRIMARY KEY (`uid`)
);
CREATE TABLE `card_net` (
`time`  varchar(255) NOT NULL ,
`uid`  varchar(8) NOT NULL ,
PRIMARY KEY (`uid`)
);

