//app.js
App({
  globalData: {
    data:[
      {
        timestamp:0,
        imageUrl:"",
        humanExist:false,
        bookExist:false,
        occupied:false,
      },
      {
        timestamp:0,
        imageUrl:"",
        humanExist:false,
        bookExist:false,
        occupied:false,
      },
      {
        timestamp:0,
        imageUrl:"",
        humanExist:false,
        bookExist:false,
        occupied:false,
      },
      {
        timestamp:0,
        imageUrl:"",
        humanExist:false,
        bookExist:false,
        occupied:false,
      },
      {
        timestamp:0,
        imageUrl:"",
        humanExist:false,
        bookExist:false,
        occupied:false,
      },
      {
        timestamp:0,
        imageUrl:"",
        humanExist:false,
        bookExist:false,
        occupied:false,
      }
    ],
    timestamp:0,
    imageUrl:"",
    humanExist:false,
    bookExist:false,
    occupied:false,
  },

  getUrl:function () {
    var d = this.globalData;
      wx.request({
        url: 'https://images.bemfa.com/cloud/v1/get/', 
        method:'GET',
        data: {
          uid:'1823ec2d13e9d8c69b196d6822d8f17d',
          topic:'esp32CAM'
        },
        success: function(res) {
        d.data[0].imageUrl = res.data.data[0].url;
        console.log("已获取图像地址：",d.data[0].imageUrl);
        }
    })
  },

  detectThings:function () {
    var d = this.globalData;
    wx.request({
       url: 'https://api-cn.faceplusplus.com/imagepp/beta/detectsceneandobject', 
      method:'POST',
      header: { 
        'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
      },
      data: {
        api_key: 'uutPiTYKdP1jQb_tQ-6zguHSdn6E4WIb',
        api_secret:'UbBXZ-lSP1HM1FMgSuECE_zPXQgaalSV',
        image_url:d.data[0].imageUrl ,
      },
      success: function(res) {
        console.log("进行图像检测，图像中包含：",res.data.objects[0].value);
        var thing = res.data.objects[0].value;
        if (thing == "books" || thing == "computer" || thing == "clothes" || thing == "bags"){
          d.data[0].bookExist = true;
        }
      }
    })
  },

  detectHuman:function () {
    var d = this.globalData;
    wx.request({
      url: 'https://api-cn.faceplusplus.com/humanbodypp/v1/detect', 
      method:'POST',
      header: { 
        'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
      },
      data: {
      api_key: 'uutPiTYKdP1jQb_tQ-6zguHSdn6E4WIb',
      api_secret:'UbBXZ-lSP1HM1FMgSuECE_zPXQgaalSV',
      image_url: d.data[0].imageUrl ,
      },
      success: function(res) {
      var confidence = res.data.humanbodies[0].confidence;
      console.log("进行人体检测，检测可信度：",confidence);
      if (confidence >=70){
        d.data[0].humanExist = true;
      }
      }
  })
  },

  judge:function () {
    var data = this.globalData;
    if(!data.data[0].humanExist && data.data[0].bookExist){
      data.data[0].timestamp = data.data[0].timestamp + 5;
    }
    else if(data.data[0].humanExist){
      data.data[0].timestamp = 0;
    }
    if(data.data[0].timestamp >= 30){
      data.data[0].occupied = true;
    }
  },

  onLaunch: function () {
    var that = this;
  
    var logs = wx.getStorageSync('logs') || [];
    logs.unshift(Date.now());
    wx.setStorageSync('logs', logs);

    wx.login({
      success: res => {
      }
    });


    setInterval(() => {
      that.getUrl();

      setTimeout(() => {
        that.detectThings();
        that.detectHuman();
      }, 1000);

      setTimeout(() => {
        that.judge();
      }, 3000);
    }, 1000*60*5);

  },

})
