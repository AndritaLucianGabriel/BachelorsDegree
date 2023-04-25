document.addEventListener("DOMContentLoaded", function () {
    const button = document.getElementById("myButton");
    const content = document.getElementById("content");
    var a = 0;

    button.addEventListener("click", function () {
        content.innerHTML = "This content was updated " + a +" times by the button click.";
        a++;
    });
});
