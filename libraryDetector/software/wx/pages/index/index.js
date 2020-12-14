const app = getApp();

Page({
  data:{
    sites:[{
        state:"开启",
        _timestamp:0,
        _humanExist:false,
        _bookExist:false,
        _occupied:false,
      },
      {
        state:"关闭",
        _timestamp:0,
        _humanExist:false,
        _bookExist:false,
        _occupied:false,
      },
      {
        state:"关闭",
        _timestamp:0,
        _humanExist:false,
        _bookExist:false,
        _occupied:false,
      },
      {
        state:"关闭",
        _timestamp:0,
        _humanExist:false,
        _bookExist:false,
        _occupied:false,
      },
      {
        state:"关闭",
        _timestamp:0,
        _humanExist:false,
        _bookExist:false,
        _occupied:false,
      },
      {
        state:"关闭",
        _timestamp:0,
        _humanExist:false,
        _bookExist:false,
        _occupied:false,
      },
    ],
    _state:"未选择",
    _timestamp:0,
    _humanExist:null,
    _bookExist:null,
    _occupied:null,
    chosen:0,
  },

  chose1:function () {
    var d = this.data.sites[0];
    this.setData({
      _state:d.state,
      _timestamp:d._timestamp,
      _humanExist:d._humanExist,
      _bookExist:d._bookExist,
      _occupied:d._occupied,
      chosen:1,
    })
  },
  chose2:function () {
    var d = this.data.sites[1];
    this.setData({
      _state:d.state,
      _timestamp:d._timestamp,
      _humanExist:d._humanExist,
      _bookExist:d._bookExist,
      _occupied:d._occupied,
      chosen:2,
    })
  },
  chose3:function () {
    var d = this.data.sites[3];
    this.setData({
      _state:d.state,
      _timestamp:d._timestamp,
      _humanExist:d._humanExist,
      _bookExist:d._bookExist,
      _occupied:d._occupied,
      chosen:3,
    })
  },
  chose4:function () {
    var d = this.data.sites[3];
    this.setData({
      _state:d.state,
      _timestamp:d._timestamp,
      _humanExist:d._humanExist,
      _bookExist:d._bookExist,
      _occupied:d._occupied,
      chosen:4,
    })
  },
  chose5:function () {
    var d = this.data.sites[4];
    this.setData({
      _state:d.state,
      _timestamp:d._timestamp,
      _humanExist:d._humanExist,
      _bookExist:d._bookExist,
      _occupied:d._occupied,
      chosen:5,
    })
  },
  chose6:function () {
    var d = this.data.sites[5];
    this.setData({
      _state:d.state,
      _timestamp:d._timestamp,
      _humanExist:d._humanExist,
      _bookExist:d._bookExist,
      _occupied:d._occupied,
      chosen:6,
    })
  },

  unchose:function () {
    this.setData({
      _state:"未选择",
      _timestamp:0,
      _humanExist:null,
      _bookExist:null,
      _occupied:null,
      chosen:0,
    })
  },

  clear:function () {
    var gd = app.globalData;
    if(this.data.chosen != 0){
      var d = this.data.chosen - 1;
      gd.data[d].timestamp = 0;
      gd.data[d].imageUrl = "";
      gd.data[d].humanExist = false;
      gd.data[d].bookExist = false;
      gd.data[d].occupied = false;
    }
  },

  back:function () {
    
  },

  onShow:function () {
    var gd = app.globalData.data;
    var that = this;
    var d = that.data.sites;
    setInterval(() => {
      d[0]._timestamp = gd[0].timestamp;
      d[0]._humanExist = gd[0].humanExist;
      d[0]._bookExist = gd[0].bookExist;
      d[0]._occupied = gd[0].occupied;
    }, 3*60*1000);
  }

})