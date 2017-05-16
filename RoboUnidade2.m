% Implementação da movimentação de um robo treinado por uma MLP
% Autores : Alex Alves de Medeiros
%           Thúlio Mattheus Pereira de Sousa

function [ ] = RoboUnidade2( serPort )

    clc;

    % Tempo máximo de simulação
	DuracaoMaxima = 1200;
    % Marcar o inicio do tempo
    inicio = tic;

    SetFwdVelAngVelCreate(serPort,0,0) %Inicia o robo



%   javaaddpath('C:\Program Files\swipl\lib\jpl.jar');
    x = org.jpl7.Query('consult(''testeROBO.pl'')');
    x.hasSolution;

    iteracao = 0;
    
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

        [a, b] = genOverhead(serPort);
        plot(a, b, 'b*');

%        fprintf('Eixo X = %.1f   Eixo Y = %.1f\n\n', a, b);        
        fprintf('Frente = %.1f   Direita = %.1f   Esquerda = %.1f   Tras = %.1f\n\n', Frente, Direita, Esquerda, Tras);
        fprintf('Roda da Esquerda = %.1f\nRoda da Direita = %.1f\n\n', VelocidadeRodaEsquerda, VelocidadeRodaDireita);

        SetDriveWheelsCreate(serPort,VelocidadeRodaDireita,VelocidadeRodaEsquerda);
        
        iteracao = iteracao + 1;
        
    end
end
