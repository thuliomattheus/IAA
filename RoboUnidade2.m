% Implementação da movimentação de um robo treinado por uma MLP
% Autor : Alex Alves de Medeiros
%         Thúlio Mattheus Pereira de Sousa

function [ ] = RoboUnidade2( serPort )
    % Tempo máximo de simulação
	DuracaoMaxima = 1200;
    % Marcar o inicio do tempo
    inicio = tic;
    % Velocidade  (m/s)
%    velocidade= 0.5;

%    r = 0.16; %raio do robo
%    L = 0.258; %distancia entre as rodas

    SetFwdVelAngVelCreate(serPort,0,0) %Inicia o robo







%    javaaddpath('C:\Program Files\swipl\lib\jpl.jar');
    x = org.jpl7.Query('consult(''testeROBO.pl'')');
    x.hasSolution;

    while toc(inicio) < DuracaoMaxima

        % 1 right sonar, 2 – front, 3 – left, 4 – back.
        Frente = ReadSonarMultiple(serPort,2);        
        Esquerda = ReadSonarMultiple(serPort,3);
        Direita = ReadSonarMultiple(serPort,1);
        Tras = ReadSonarMultiple(serPort,4);
        
        if isempty(Frente)==1
            Frente = 0;
        end        
        if isempty(Esquerda)==1
            Esquerda = 0;
        end
        if isempty(Direita)==1
            Direita = 0;
        end
        if isempty(Tras)==1
            Tras = 0;
        end




        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PROLOG %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        c = {{strcat('pRobo(X,Y,', num2str(Frente), ',' , num2str(Direita), ',' , num2str(Esquerda), ',' , num2str(Tras) , ').')}};

        for i=1:length(c)

            disp(c{i}{:})
            x = org.jpl7.Query(c{i}{:});
            r = x.allSolutions;

            for j=1:length(r)
                a = r(j).values.toArray;
                for k=1:r(j).size
                    if k==1
                        VelocidadeRodaEsquerda = str2double(char(a(k).toString));
                    elseif k==2
                        VelocidadeRodaDireita = str2double(char(a(k).toString));
                    end
                end
            end
        end

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%        VelocidadeRodaEsquerda = 0.3;
%        VelocidadeRodaDireita = 0;
        fprintf('Frente = %.1f   Direita = %.1f   Esquerda = %.1f   Tras = %.1f\n\n', Frente, Direita, Esquerda, Tras);
        fprintf('Roda da Esquerda = %.1f\nRoda da Direita = %.1f\n\n', VelocidadeRodaEsquerda, VelocidadeRodaDireita);
        
        
        % Calculando as velocidades angulares 
%        we = VelocidadeRodaEsquerda/r;
%        wd = VelocidadeRodaDireita/r;
        % Faz o robo andar a partir das velocidades lineares das rodas
        SetDriveWheelsCreate(serPort,VelocidadeRodaDireita,VelocidadeRodaEsquerda)
        % Movimenta através das velocidades angulares
        % SetDriveWheelsCreate(serPort,wd,we); 

%        velocidade = (r/2)*(wd+we);
%        w = (r/L)*(wd-we);     
    end
end
