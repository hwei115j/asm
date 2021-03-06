# ASM

簡單的組譯器

### 此計算機由下列硬體元件組成:

     4096個字的記憶體，其中每個字16位元
     9個暫存器: AR, PC, DR, AC, IR, TR, OUTR, INPR及SC
     7個正反器: I, S, E, R, IEN, FGI及FGO
     2個解碼器: 一個3x8運算解碼器及一個4x16定時解碼器
     一個16位元公用匯流排
     控制邏輯閘
     連接於AC之輸入的加法器和邏輯電路

### 指令集

|符號 | 十六進製碼 | 說明|
|:-----:|:-----------------:|:----------------------------------------|
||`I=0   I=1`|`記憶存取指令`|
|AND    | 0xxx     8xxx|        將記憶字與 AC 作 AND|
|ADD    | 1xxx     9xxx|        將記憶字加至 AC|
|LDA     | 2xxx     Axxx|        載入記憶字至 AC|
|STA     |3xxx     Bxxx|        將 AC 內容儲存於記憶體中|
|BUN     | 4xxx    Cxxx|        無條件分支|
|BSA      |5xxx    Dxxx|        分支並保存回返地址|
|ISZ       |6xxx    Exxx |       遞增且若為零則跨越|
||`暫存器存取指令`||
|CLA         |7800|            清除 AC|
|CLE         |7400|            清除 E|
|CMA         |7200|           變補 AC|
|CME         |7100|            變補 E|
|CIR         |7080|            將 AC 與 E 向右循環|
|CIL         |7040|            將 AC 與 E 向左循環|
|INC         |7020|            遞增 AC|
|SPA         |7010|            若 AC 為正則跨越次一指令|
|SNA        |7008|            若 AC 為負則跨越次一指令|
|SZA         |7004|            若 AC 為零則跨越次一指令|
|SZE         |7002|            若 E 為零則跨越次一指令|
|HLT         |7001|            暫停計算機|
||`輸入-輸出指令`||
|INP         |F800|            輸入字元至AC|
|OUT         |F400|           從AC輸出字元|
|SKI         |F200|            依輸入旗標跨越|
|SKO         |F100|            依輸出旗標跨越|
|ION         |F080|            啟動岔斷|
|IOF         |F040|            停止岔斷|
