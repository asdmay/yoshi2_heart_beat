/***********************************************************
	2015
	Yoshi Yoshi Heart Beat
	Shun Naito, Maya Ashida, Kosuke Matsuno, Seiya Iwasaki
***********************************************************/


/* 外部ライブラリ */
#include <CapacitiveSensor.h>


/* マクロ */
#define NOISE 10
#define BUFFER_LENGTH 5
#define DEBUG


/* 静電容量センサ */
const int transPin = 2;			// 送信用デジタルピンの番号
const int recievePin = 3;		// 受信用デジタルピンの番号
const int thresholdHigh = 200;		// タッチ状態を識別するしきい値(上)
const int thresholdLow = 150;		// タッチ状態を識別するしきい値(下)
const int toSmall = 1;			// 検出された値が大きすぎる場合
long capValue = 0;			// 検出された静電容量の値
boolean touchStatus = false;		// タッチ状態
boolean preTouchStatue = false;		// 前回のタッチ状態
CapacitiveSensor *CapSensor = new CapacitiveSensor(transPin, recievePin);


/* バッファ (スムージング処理用) */
long cap_buffer[BUFFER_LENGTH];
int bufferIndex = 0;

/* モーター */
const int motorPin = 9;

/*-----------------------------------------------------------------*/

void setup() {
	/* CapacitiveSensor 初期化 */
	CapSensor->reset_CS_AutoCal();			// センサのキャリブレーション
	CapSensor->set_CS_AutocaL_Millis(0xFFFFFFFF);	// オートキャリブレーションのオフ
	for(int i = 0; i < BUFFER_LENGTH; i++){
		SenseCap();
	}

	/* Serial通信開始 */
	Serial.begin(9600);

        /* モーター */
        pinMode(motorPin,OUTPUT); //信号用ピン
}


/*-----------------------------------------------------------------*/

void loop() {

	// 静電容量の検出
	capValue = SenseCap() / toSmall;

	// タッチ状態の判定
	if(capValue > thresholdHigh){
		touchStatus = true;
	}else if(capValue < thresholdLow){
		touchStatus = false;
	}else{
		touchStatus = preTouchStatue;
	}
	preTouchStatue = touchStatus;

        // タッチされた場合の処理
        if(touchStatus == true){
          digitalWrite(motorPin,HIGH);
          delay(100);
          digitalWrite(motorPin,LOW);
          delay(100);
        }
          

	// デバッグ用
    #ifdef DEBUG
        Serial.println();
        Serial.print("capValue : ");
        Serial.print(capValue);
        Serial.print(", ");
        Serial.print(touchStatus);
        Serial.println();
    #endif

}


/*-----------------------------------------------------------------*/

long SenseCap(){
    long raw = CapSensor->capacitiveSensor(NOISE);
    cap_buffer[bufferIndex] = raw;

    long FilterCapValue = smoothByMeanFilter(cap_buffer);
    bufferIndex = (bufferIndex + 1) % BUFFER_LENGTH;
    
    return FilterCapValue;
}

/*-----------------------------------------------------------------*/

/* Meanフィルタ(平均化)によるスムージング */
long smoothByMeanFilter(long *box){
    long sum = 0;
    for(int i = 0; i < BUFFER_LENGTH; i++){
        sum += box[i];
    }
    return (long)(sum / BUFFER_LENGTH);
}
