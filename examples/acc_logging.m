%% Connect to sensor
metawear('disconnect_all');
sensor1 = metawear('connect','CA:8A:CF:D8:BC:7C');
sensor2 = metawear('connect','F2:C0:45:7D:3D:43');
sensor3 = metawear('connect','F9:60:2C:3E:8B:13');
metawear('set_connection_parameters',sensor1,7.5, 7.5, 0,10000);
metawear('set_connection_parameters',sensor2,7.5, 15, 0,10000);
metawear('set_connection_parameters',sensor3,7.5, 15, 0,10000);
%%
metawear('configure_accelerometer_sample',sensor1,25);
metawear('configure_accelerometer_range',sensor1,4);
metawear('configure_accelerometer_config',sensor1);
metawear('enable_accelerometer',sensor1);

metawear('configure_accelerometer_sample',sensor2,25);
metawear('configure_accelerometer_range',sensor2,4);
metawear('configure_accelerometer_config',sensor2);
metawear('enable_accelerometer',sensor2);

metawear('configure_accelerometer_sample',sensor3,25);
metawear('configure_accelerometer_range',sensor3,4);
metawear('configure_accelerometer_config',sensor3);
metawear('enable_accelerometer',sensor3);


signal3 = metawear('subscribe_acc',sensor3,'LOG');
signal2 = metawear('subscribe_acc',sensor2,'LOG');
signal1 = metawear('subscribe_acc',sensor1,'LOG');

metawear('clear_logging',sensor1);
metawear('clear_logging',sensor2);
metawear('clear_logging',sensor3);

%%
metawear('start_logging',sensor1);
metawear('start_logging',sensor2);
metawear('start_logging',sensor3);
pause(3);
metawear('stop_logging',sensor1);
metawear('stop_logging',sensor2);
metawear('stop_logging',sensor3);

metawear('download_logging',sensor1);
metawear('download_logging',sensor2);
metawear('download_logging',sensor3);


[epoch_1,x_1,y_1,z_1] = metawear('query',sensor1,signal1);
[epoch_2,x_2,y_2,z_2] = metawear('query',sensor2,signal2);
[epoch_3,x_3,y_3,z_3] = metawear('query',sensor3,signal3);
