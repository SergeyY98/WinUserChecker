const express = require('express');
const { spawn } = require('child_process');
const path = require('path');

const app = express();

app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

const { isUserExists } = require('bindings')('userChecker');

app.post('/checkUser', (req, res) => {
    const username = req.body.username;
    const userExists = isUserExists(username);

    if (userExists) {
        res.send('Пользователь есть <br/><a href="/">Назад</a>');
    } else {
        res.send('Пользователя нет <br/><a href="/">Назад</a>');
    }
});

app.listen(3000, () => {
    console.log('Server is running on http://localhost:3000');
    spawn('cmd', ['/c', 'start', 'http://localhost:3000']);
});