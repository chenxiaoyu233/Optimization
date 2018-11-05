// Belong: k-defctive
// Function : 根据实验数据自动生成各个情况的折线图

// google图表库
google.charts.load('current', {'packages':['line']});
google.charts.setOnLoadCallback(drawChart);

// 创建并绘制一张折线图
// @param dic: 包含数据的字典.
// 格式:
// {
//  "Base": [],
//  "RDS": [],
//  "Simple": [],
//  “K": [],
//  "Case": "case-name" 
// }
function drawLineChart(dic) {
    // 初始化
    var data = new google.visualization.DataTable();

    // 添加表头
    // x轴
    data.addColumn('number', 'K');
    // 几种不同的情况
    data.addColumn('number', 'Base');
    data.addColumn('number', 'RDS');
    data.addColumn('number', 'Simple');

    // 添加数据
    for (var i = 0; i < dic.K.length; i++) {
        data.addRow([dic.K[i], dic.Base[i], dic.RDS[i], dic.Simple[i]]);
    }

    // 添加参数
    var options = {
        chart: {
            title: dic.Case,
            subtitle: ''
        },
        width: 900,
        height: 500
    };

    // 绘制
    // 在html中生成对应的div
    var div = document.createElement('div');
    document.body.appendChild(div);
    // 以div为基础进行图形绘制
    var chart = new google.charts.Line(div);
    chart.draw(data, google.charts.Line.convertOptions(options));
}

// 遍历目录
function readDirection() {
    var input = document.getElementsByClassName("input");
    console.log(input);
}

// Debug
function drawChart() {
    /*dic = {
        "Base": [1, 2, 3, 4, 5],
        "RDS": [2, 3, 4, 6, 10],
        "Simple": [4, 5, 9, 10, 12],
        "K": [1, 2, 3, 4, 5],
        "Case": "test"
    };
    drawLineChart(dic);*/
    readDirection();
}