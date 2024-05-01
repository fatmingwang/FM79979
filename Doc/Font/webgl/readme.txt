1.
	create a canvas
        const canvas = document.getElementById('myCanvas');
2.
	get contenxt
        const ctx = canvas.getContext('2d');
3.
	set font data and get width and height
        ctx.font = '20px Arial';
	let l_strTestCharacter = "qoo";
	let metrics = ctx.measureText(l_strTestCharacter);
	let fontHeight = metrics.fontBoundingBoxAscent + metrics.fontBoundingBoxDescent;
	let actualHeight = metrics.actualBoundingBoxAscent + metrics.actualBoundingBoxDescent;
	console.log("font height:"+fontHeight+" acctualHeight:"+actualHeight);
        var width = ctx.measureText(l_strTestCharacter).width;
4.
	fill text into canvas's context
        ctx.fillText(htmlText, 0, width);
5.	get pixel data


	var imgd = context.getImageData(x, y, width, height);
	var pix = imgd.data;
	
	// Loop over each pixel and invert the color.
	for (var i = 0, n = pix.length; i < n; i += 4) {
	    pix[i  ] = 255 - pix[i  ]; // red
	    pix[i+1] = 255 - pix[i+1]; // green
	    pix[i+2] = 255 - pix[i+2]; // blue
	    // i+3 is alpha (the fourth element)
	}
	
	// Draw the ImageData at the given (x,y) coordinates.
	context.putImageData(imgd, x, y);