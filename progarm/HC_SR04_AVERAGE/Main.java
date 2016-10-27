package com.test.data;

import java.util.Random;

public class Main {

	static int random[] = { 40, 50, 51, 52, 100};
	static int data[] = { 1, 2, 3, 4, 5, 6, 4, 2, 1, 5, 9, 10};
	static Random r = new Random(System.currentTimeMillis());
	
	static class Serial
	{

		public static void print(int j) {
			System.out.print(j);
		}

		public static void println(int i) {
			System.out.println(i);
		}

		public static void print(String string) {
			System.out.print(string);
		}
		
	}

	public static void main(String[] arg) {
		final int averLength = 100;
		final int dis_err = 10;
		final double maxDis = 0.2;
		final int maxDataArray = 5;
	    int aver[] = new int[averLength];
	    int storageAver[] = new int[averLength];
	    for (int i = 0; i < averLength; ++i) {
	      aver[i] = readHCSR04();
	    }
	    bubble(aver,averLength);
	    int sum = -dis_err;
	    int count = 1;
	    int start = 0;
	    int aimCount = 0;
	    for (int i = 0; i < averLength; ++i) {
	      int _aver = sum / count;
	      if(aver[i] - _aver < dis_err){
	        sum += aver[i];
	        ++count;
	      }
	      else{
	        start = i;
	        storageAver[i] = aver[i];
	        sum = aver[i];
	        count = 1;
	        ++aimCount;
	      }
	      int lastAver = _aver;
	      _aver = sum / count;
	      if(lastAver == _aver){
	    	  storageAver[i] = _aver;
	      }else{
	    	  for(int j = start;j <= i;++j){
	  	        storageAver[j] = _aver;
	  	        Serial.print(j);
	  	        Serial.print("-");
	  	        Serial.println(storageAver[j]);
	  	      }
	      }
	    }
	    final int _aimCount = aimCount;
	    if(_aimCount <= maxDataArray){
	    	int res[][] = new int[_aimCount][2];
		    start = 0;
		    for(int i = 0;i < _aimCount;++i)
		    {
		      res[i][0] = storageAver[start];
		      int j = start;
		      count = 0;
		      for(; j <= averLength;++j)
		      {
		        if(j != averLength && storageAver[j] == res[i][0])
		        {
		          ++count;
		        }else{
		          res[i][1] = count;
		          start = j;
		          break;
		        }
		      }
		    }
		    
		    int maxCount = 0;
		    for(int i = 0;i < aimCount;++i)
		    {
		    	if(res[i][1] > res[maxCount][1]){
		    		maxCount = i;
		    	}
		      Serial.print(res[i][0]);
		      Serial.print("-");
		      Serial.println(res[i][1]);
		    }
		    int errorCount = -1;
		    double errorDis = -1;
		    for(int i = 0;i < aimCount;++i){
		    	if(i != maxCount){
		    		if(res[i][1] == res[maxCount][1]){
		    			errorCount = i;
		    			errorDis = 0;
		    			break;
		    		}
		    		double bigCount = (res[i][1] > res[maxCount][1])?(res[i][1]):(res[maxCount][1]);
		    		double smallCount = (res[i][1] < res[maxCount][1])?(res[i][1]):(res[maxCount][1]);
		    		errorDis = (bigCount - smallCount) / bigCount;
		    		if(errorDis <= maxDis){
		    			errorCount = i;
		    			break;
		    		}
		    	}
		    }
		    if(errorCount == -1){
		    	Serial.print("max count is : " + res[maxCount][0] + "-" + res[maxCount][1]);
		    }else{
				Serial.print("no result,error is: " + res[maxCount][0] + "-"
						+ res[maxCount][1] + " , " + res[errorCount][0] + "-"
						+ res[errorCount][1] + " , " + errorDis);
		    }
	    }else{
	    	Serial.print("no result,to more data");
	    }
	}

	/* 定义两个参数：数组首地址与数组大小 */
	public static void bubble(int a[], int n) 
	{
		int i, j, temp;
		for (i = 0; i < n - 1; i++)
			for (j = i + 1; j < n; j++)
				/* 注意循环的上下限 */
				if (a[i] > a[j]) {
					temp = a[i];
					a[i] = a[j];
					a[j] = temp;
				}
	}

	static int callCount = 0;
	public static int readHCSR04() {
		++callCount;
		int type = 0;
		if(callCount <= 48){
			type = 1;
		}else if(callCount <= 96){
			type = 2;
		}else if(callCount <= 100){
			type = 3;
		}else{
			type = 4;
		}
		switch(type++){
		case 1:
			return 153;
		case 2:
			return 162;
		case 3:
			return 172;
		case 4:
			return 158;
		}
		return 0;
//		return data[(Math.abs(r.nextInt())) % data.length]
//				+ random[(Math.abs(r.nextInt())) % random.length];
	}

}
/*int start = 0;
if(0 == i){
	start = i;
	storageAver[i] = aver[i];
	sum += aver[i];
	count = 1;
}else{
	int _aver = sum/count;
	if(aver[i] - _aver <= dis_err){
		sum += aver[i];
		++count;
		_aver = sum/count;
		for(int j = start;j < i;++j){
			storageAver[j] = _aver;
		}
	}else{
		start = i;
	}
}*/