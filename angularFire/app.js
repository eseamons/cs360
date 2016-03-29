var app = angular.module("sampleApp", ["firebase"]);
app.controller("SampleCtrl", function($scope, $firebaseArray) {
  var ref = new Firebase("https://radiant-fire-9507.firebaseio.com/messages");

  $scope.messages = $firebaseArray(ref);

  $scope.addMessage = function() {
    $scope.messages.$add({
      text: $scope.newMessageText
    });

    $scope.newMessageText = null;

  };

});