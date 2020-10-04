const express = require('express');
const fs = require('fs');
const app = express();
const path = require('path');
var spawn = require("child_process").spawn;
const bodyParser = require('body-parser');

app.set('view engine', 'ejs')

app.use(express.static(__dirname + '/views'));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(express.static(__dirname + '/static'));

app.get('/',(req,res)=>{
    res.sendFile(path.join(__dirname, 'static', 'index.html'));
});

app.get('/info',(req,res)=>{
    res.sendFile(path.join(__dirname, 'static', 'info.html'));
});

let requirements = {cost:null,pollutant:null,moisture:null,temperature:null};
app.get('/main',(req,res)=>{
    res.render('index');
});

app.post('/process',(req,res)=>{
    // console.log(req.body);
    requirements.cost = req.body.costlist;
    requirements.pollutant = req.body.polllist;
    requirements.moisture = req.body.moistlist;
    requirements.temperature = req.body.templist;
    console.log(requirements);
    var process = spawn('python',["./ml.py",
    requirements.pollutant,requirements.cost,requirements.moisture,requirements.temperature]);

     // with arguments and send this data to res object 
     process.stdout.on('data', function(data) { 
        res.redirect('/result');
    } )
});

app.get('/result',(req,res)=>{
    var pythonData = [];
    const python = spawn('python',['ML_model.py']);
    python.stdout.on('data',function(data){
        pythonData.push(data);
        console.log(data);
    });
    
    python.on('close',(code)=>{
        res.send(pythonData)
        console.log(pythonData);
    })
});

app.listen(3000,()=>console.log("Listening at port 3000..."));
