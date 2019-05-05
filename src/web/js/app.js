

angular.module('iothon', [])
.controller('IothonController', function ($scope, $http) {
    var todoList = this;
    todoList.todos = [
        { text: 'learn AngularJS', done: true },
        { text: 'build an AngularJS app', done: false }];

    todoList.addTodo = function () {
        todoList.todos.push({ text: todoList.todoText, done: false });
        todoList.todoText = '';
    };

    todoList.remaining = function () {
        var count = 0;
        angular.forEach(todoList.todos, function (todo) {
            count += todo.done ? 0 : 1;
        });
        return count;
    };

    todoList.archive = function () {
        var oldTodos = todoList.todos;
        todoList.todos = [];
        angular.forEach(oldTodos, function (todo) {
            if (!todo.done) todoList.todos.push(todo);
        });
    };
    $http.get('http://localhost:12345/api')
        .then(function (response) {
            $scope.jsondata = response.data;
            console.log("status:" + response.data);
        }).catch(function (response) {
            console.error('Error occurred:', response.status, response.data);
        }).finally(function () {
            console.log("Task Finished.");
        });
});

