SELECT * FROM xungao.user;

create table Camera(
	ip varchar(15) not null auto_increment primary key,
    username varchar(50) not null,
    password varchar(50) not null
);

create table alarms(
	id int not null auto_increment primary key,
    ip varchar(15) not null,
    type int not null,
    time timestamp not null default current_timestamp,
    image varchar(255) not null
);
insert into alarms(ip, type, image) values("192.168.1.65", 2, "/a.png");
insert into alarms(ip, type, image) values("192.168.1.66", 1, "/45.png");
insert into alarms(ip, type, image) values("192.168.1.65", 4, "/7852.png");

select * from alarms;
insert_str = "insert into alarms(ip, type, time, path) values('192.168.1.65', '1', '20191019', '/root/out/a.jpg')"
select count(*) from alarms;