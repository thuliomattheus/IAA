function testeFUZZY(serPort)
clc

fismat = readfis('testeROBO');


    while true

        [x y]= genOverhead(serPort);
        plot(x,y);

        distSonar = genSonar(serPort);

        frente= distSonar(1,1);
        esquerda= distSonar(1,2);
        tras= distSonar(1,3);
        direita=distSonar(1,4);

        disp(['Frente = [' num2str(frente) ']']);
        disp(['Esquerda = [' num2str(esquerda) ']']);
        disp(['Trás = [' num2str(tras) ']']);
        disp(['Direita = [' num2str(direita) ']']);

        v = evalfis([frente,direita,esquerda,tras],fismat);

        we = v(1,1);
        wd = v(1,2);


        if we>0.5
            we=0.5;
        end
        if we<-0.5
            we=-0.5;
        end


        if wd>0.5
            wd=0.5;
        end

        if wd<-0.5
            wd=-0.5;
        end

        SetDriveWheelsCreate(serPort,wd,we);

    end
end

