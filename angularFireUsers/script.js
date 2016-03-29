var app = angular.module("app", ["firebase"]);
app.controller("mainCtrl", function($scope, $firebaseArray, $window) {
  var ref = new Firebase("https://radiant-fire-9507.firebaseio.com/users");

  $scope.users = $firebaseArray(ref);

  $scope.addNew = function (user) {
  	if (!user.avatarUrl) {
      user.avatarUrl = 'http://thealmanac.org/assets/img/default_avatar.png';
    }

    $scope.users.$add({
      name: user.name,
      avatarUrl: user.avatarUrl
    }); /* [1] */
    
    user.name = ''; /* [2] */
    user.avatarUrl = '';
  };

  $scope.delete = function(user) {

    var decision = $window.confirm('Are you sure you want to delete this user? If so, click Ok.');
    if(decision) {
      $scope.users.$remove(user);
    }
  }

});