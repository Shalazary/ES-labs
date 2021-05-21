- lab1   
Постоянно моргать светодиодами с нечетными номерами. После нажатия на кнопку начинают моргать светодиоды с четными номерами.  
- lab2  
Постоянно моргать светодиодами с нечетными номерами. Нажатие на кнопку переключает источник тактирования MCU с внешнего (HSE) на внутренний (HSI) и обратно.  
- lab3  
Считывать показания с переменного резистора, используя АЦП. Отображать уровень считанного сигнала, используя светодиоды. Чем больше уровень, тем больше светодиодов загорается.  
- lab4  
Переписать с использованием прерываний.  
Постоянно моргать светодиодами с нечетными номерами с интервалом 4 сек. После нажатия на кнопку начинают моргать светодиоды с четными номерами с интервалом 2 сек.  
- lab5  
Переписать с использованием прерываний. Реализовать антидребезг для кнопки.  
Постоянно моргать светодиодами с нечетными номерами с интервалом 4 сек. После нажатия на кнопку начинают моргать светодиоды с четными номерами с интервалом 2 сек.  
- lab6  
Часть первая: написать управление яркостью светодиодов Led1..Led8 без использования аппаратного ШИМ. Нажатие на кнопку увеличивает коэфф. заполнения (яркость) на 20% циклично.  
Часть вторая: Написать управление яркостью светодиодов Led1..Led8 с использованием модуля ШИМ микроконтроллера. Нажатие на кнопку увеличивает яркость на 20% циклично. Попробовать с разной частотой ШИМ.  
- lab7  
Модифицировать первую задачу следующим образом:  
Задача предполагает, что существует два различных режима работы программы. 1-ый режим, когда кнопка нажата. 2-ой режим, когда кнопка не нажата. Необходимо запоминать текущий режим работы, используя встроенную в микроконтроллер флеш-память. После перезагрузки микроконтроллера режим работы должен восстанавливаться из флеш-памяти.
