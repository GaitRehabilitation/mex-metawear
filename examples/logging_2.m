%%

metawear('disconnect_all');
%% Connect to sensor

 sensor1.mac = 'CA:8A:CF:D8:BC:7C';
%sensor2.mac = 'F9:60:2C:3E:8B:13';
sensor3.mac = 'F6:92:07:98:01:64';
%sensor4.mac = 'F4:2C:CE:68:20:4F';
sensor5.mac = 'C9:70:5D:D7:99:78';
%sensor6.mac = 'DB:04:F4:40:20:56';
%sensor7.mac = 'F2:C0:45:7D:3D:43';
%sensor8.mac = 'D9:0A:8A:88:7A:4A';
%sensor9.mac = 'DE:07:3D:9E:B6:C4';
%sensor10.mac = 'E3:F2:37:02:7A:70';

sensors = [sensor1,sensor3,sensor5];
%%
sz = size(sensors);
for i = 1:sz(2)
sensors(i).mac = metawear('connect',sensors(i).mac);
if(strcmp(sensors(i).mac,'FF:FF:FF:FF:FF:FF') == 0)
fprintf("rest sensor: %s\n",sensors(i).mac)
metawear('reset',sensors(i).mac);
metawear('disconnect_all');
end
        end
metawear('clear_state_data')
%%
sz = size(sensors);
for i = 1:sz(2)
sensors(i).mac = metawear('connect',sensors(i).mac);
if(strcmp(sensors(i).mac,'FF:FF:FF:FF:FF:FF') == 0)
fprintf("configuring sensor: %s\n",sensors(i).mac)
metawear('teardown',sensors(i).mac);
metawear('clear_logging',sensors(i).mac);

metawear('configure_accelerometer_sample',sensors(i).mac,25);
metawear('configure_accelerometer_range',sensors(i).mac,4);
metawear('configure_accelerometer_config',sensors(i).mac);
metawear('enable_accelerometer',sensors(i).mac);

metawear('configure_gyro_sample',sensors(i).mac,25);
metawear('configure_gyro_config',sensors(i).mac);
metawear('enable_gyro',sensors(i).mac);

sensors(i).acc = metawear('subscribe_acc',sensors(i).mac,'LOG');
sensors(i).gyro = metawear('subscribe_gyro',sensors(i).mac,'LOG');
metawear('query_handlers',sensors(i).mac);

metawear('start_logging',sensors(i).mac);

metawear('disconnect_all');
end
        end

%%
for n = 1:3600
fprintf('count: %d \n',n);
pause(1);
end

%%
metawear('disconnect_all');
sz = size(sensors);
for i = 1:sz(2)
if(strcmp(sensors(i).mac,'FF:FF:FF:FF:FF:FF') == 0)
sensors(i).mac = metawear('connect',sensors(i).mac);
if(strcmp(sensors(i).mac,'FF:FF:FF:FF:FF:FF') == 0)
metawear('set_connection_parameters',sensors(i).mac,7.5,7.5, 0,6000);
metawear('download_logging',sensors(i).mac);
metawear('query_handlers',sensors(i).mac);
[sensors(i).epoc_acc,sensors(i).x_acc,sensors(i).y_acc,sensors(i).z_acc] = metawear('query',sensors(i).mac,sensors(i).acc);
[sensors(i).epoc_gyro,sensors(i).x_gyro,sensors(i).y_gyro,sensors(i).z_gyro] = metawear('query',sensors(i).mac,sensors(i).gyro);
pause(1);
metawear('disconnect_all');
pause(1);
end
        end
end