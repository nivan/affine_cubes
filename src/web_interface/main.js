var width = 500;
var height = 500;

var svg = d3.select("body").append("svg")
    .attr("width", width)
    .attr("height", height);

function plotBins(bins){
    var xScale = d3.scale.linear().domain([0,1]).range([0,width]);
    var yScale = d3.scale.linear().domain([0,1]).range([0,height]);

    console.log(bins);

    svg.selectAll("rect")
	.data(bins)
	.enter()
	.append("rect")
	.attr("x",function(d){return xScale(d["xmin"]);})
	.attr("y",function(d){return yScale(d["ymin"]);})
	.attr("width",function(d){return xScale(d["xmax"]) - xScale(d["xmin"]);})
	.attr("height",function(d){return yScale(d["ymax"]) - yScale(d["ymin"]);})
	.attr("fill","black")
	.attr("fill-opacity","0.2")
}

function send() {
    var parameters = {"x_axis":[1,0,0,0],"y_axis":[0,1,0,0],"depth":3}

    $.ajax({
        url: 'http://localhost:18080/build_histogram_2d',
        type: 'POST',
        dataType: 'json',
        success: function (data) {
            plotBins(data);
        },
	error:function(responseData,textStatus,errorThrown){
	    alert("POST FAILED");
	},
	crossDomain:true,
        data: JSON.stringify(parameters)
    });
    
}

//console.log($.support.cors)
send()
