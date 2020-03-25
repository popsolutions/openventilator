--PORTUGUÊS--

*DECISÕES ESTRUTURAIS*
- Projetar e montar um ventilator de baixo custo (preço limite US$500,00)
- Usar peças e engenharia acessiveis a todos
- Manter a simplicidade como uma filosofia para todos
- Não confiar em um único hardware ou dispositivo, pensar na modularidade como algo essencial, nem todos tem acesso as mesmas coisas.
- A escala do projeto demanda que um número aproximado de 3Mi de aparelhos sejam construidos mundialmente nos próximos meses


*REQUISITOS TÉCNICOS*
- Projetar um Produto Minimamente Viável que:
- Tenha um fole como dispositivo de bombeamento de ar, pensamos em materiais de fácil disponibilidade como camaras de pneus.
- Utilizar peças comumente achadas na industria automotiva como motores de limpa-parabrisa
- Considerar que 12V é uma realidade visto que talvez estes respiradores precisem funcionar sem energia AC
- Um paciente crítico não poderá sofrer Bronco-trauma, portanto o desenvolvimento de um sensor integrado ao circuito eletrônico que previna este tipo de problemas
- Sistema de filtragem na exaustão para minimizar o contágio da doença em ambientes hospitalares
- Sistema de humidificação da admissão de ar a fim de não ressecar as vias respiratórias
- Ajuste proporcional de 1/1 até 1/6 no tempo de ventilação entre inspiração e expiração
- Ajuste volumétrico de ventilação para pacientes com diferentes capacidades respiratórias. Considerar até 2N/m como torque para ventilar alguem com 120Kg
- Limite máximo de ventilação de 1,6L


*MODELAGEM DA MVP*
- Motor de LimpaParabrisa 12V
- Camara de Pneu para montagem do Fole
- Uso de barras roscadas na estrutura rigida da armação
- Uso de Acrílico ou outros materiais de fácil limpeza (preferencialmente Inox porém não exlusivo dado o grau de emergência)
- Controle Electronico com Arduino (estudando a viabilidade da simplificação eletrônica)
- Sensor de pressão no Intake (vendo a viabilidade de venturi ou BMP120)
- Uso de filtro de exaustão de Ozônio e HEPA E13 para filtragem de particulas até 1µm possível adaptação de luz UV para aniquilação biológica
- Construção de valvulas PEEP e Venturi com impressora 3D, estudando a possibilidade de encontrar parceiro industrial que viavilize a injeção plástica para maior velocidade de produção


-- ENGLISH --

*STRUCTURAL DESIGNS*
- Design and assemble a low cost ventilator (limit price US $ 500.00)
- Use parts and engineering accessible to everyone
- Maintaining simplicity as a philosophy for everyone
- Do not rely on a single hardware or device, think of modularity as essential, not everyone has access to the same things.
- The scale of the project demands that an approximate number of 3Mi devices be built worldwide in the coming months

*TECHNICAL REQUIREMENTS*
- Design a Minimally Viable Product that:
- Have a bellows as an air pumping device, we think of easily available materials like tire chambers.
- Use parts commonly found in the automotive industry such as wiper motors
- Consider that 12V is a reality as these respirators may need to work without AC power
- A critical patient must not suffer Bronco-trauma, so the development of a sensor integrated in the electronic circuit that prevents this type of problems
- Exhaust filtering system to minimize the spread of disease in hospital environments
- Air intake humidification system in order not to dry out the airways
- Proportional adjustment from 1/1 to 1/6 in the ventilation time between inspiration and expiration
- Volumetric ventilation adjustment for patients with different breathing capacities. Consider up to 2N / m as torque to ventilate someone with 120Kg
- Maximum ventilation limit of 1.6L
- Considering 10 ~ 30 breathing cycles a minute

*MVP MODELING*
- Windshield Wiper Motor 12V
- Tire Chamber for Assembly of Bellows
- Use of threaded bars in the rigid frame structure
- Use of Acrylic or other materials for easy cleaning (preferably stainless steel but not exclusive given the degree of emergence)
- Electronic Control with Arduino (studying the feasibility of electronic simplification)
- Intake pressure sensor (seeing the viability of venturi or BMP120)
- Use of ozone and HEPA E13 exhaust filter to filter particles up to 1µm possible adaptation of UV light for biological annihilation
- Construction of PEEP and Venturi valves with 3D printer, studying the possibility of finding an industrial partner that makes plastic injection viable for greater production speed