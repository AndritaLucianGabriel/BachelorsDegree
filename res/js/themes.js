/* Copyright 2023 Andrita Lucian-Gabriel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

window.addEventListener("load", function () {
    const themeSwitch = document.getElementById('themeSwitch');
    //  Verific tema
    if (localStorage.getItem('theme') === 'lightTheme' || localStorage.getItem('theme') === '') {
        setTheme('lightTheme');
        themeSwitch.checked = false;
    } else if (localStorage.getItem('theme') === 'darkTheme') {
        setTheme('darkTheme');
        themeSwitch.checked = true;
    }
    function setTheme(themeName) {
      localStorage.setItem('theme', themeName);
      if (themeName == "lightTheme") {
        document.body.style.setProperty("--slider-background-color", "#b6f0ed");
        document.body.style.setProperty("--body-background-color", "white");
        document.body.style.setProperty("--header-background-color", "#87ccf5");
        document.body.style.setProperty("--header-text-color", "#444");
        document.body.style.setProperty("--chat-background-color", "#ebf2f2");
        document.body.style.setProperty("--chat-received-background-color", "#F0F2F5");
        document.body.style.setProperty("--chat-received-text-color", "#444");
        document.body.style.setProperty("--chat-sent-background-color", "#F0F2F5");
        document.body.style.setProperty("--chat-sent-text-color", "#444");
        document.body.style.setProperty("--chat-footer-background-color", "#ebf2f2");
        document.body.style.setProperty("--chat-footer-text-color", "#444");
        document.body.style.setProperty("--chat-input-background-color", "#87ccf5");
        document.body.style.setProperty("--chat-button", "white");
        document.body.style.setProperty("--bubble-background-color", "white");
        document.body.style.setProperty("--footer-background-color", "#87ccf5");
        document.body.style.setProperty("--footer-info-color", "#444");
        document.body.style.setProperty("--footer-misc-color", "#444");
      }
      else if (themeName == "darkTheme") {
          document.body.style.setProperty("--slider-background-color", "#4a4f58");
          document.body.style.setProperty("--slider-dark-background-color", "#896f07");
          document.body.style.setProperty("--body-background-color", "#4a4f58");
          document.body.style.setProperty("--header-background-color", "#2c2c2c");
          document.body.style.setProperty("--header-text-color", "#8d8d8d");
          document.body.style.setProperty("--chat-background-color", "#1f2429");
          document.body.style.setProperty("--chat-received-background-color", "#424242");
          document.body.style.setProperty("--chat-received-text-color", "#b0aeae");
          document.body.style.setProperty("--chat-sent-background-color", "#424242");
          document.body.style.setProperty("--chat-sent-text-color", "#b0aeae");
          document.body.style.setProperty("--chat-footer-background-color", "#2c3e50");
          document.body.style.setProperty("--chat-footer-text-color", "#8d8d8d");
          document.body.style.setProperty("--chat-input-background-color", "#1e1e1e");
          document.body.style.setProperty("--chat-button", "#424242");
          document.body.style.setProperty("--bubble-background-color", "#424242");
          document.body.style.setProperty("--footer-background-color", "#2d2d2d");
          document.body.style.setProperty("--footer-info-color", "#95a5a6");
          document.body.style.setProperty("--footer-misc-color", "#95a5a6");
      }
    }
    themeSwitch.addEventListener('change', () => {
      if (themeSwitch.checked) {
        setTheme("darkTheme");
      } else {
        setTheme("lightTheme");
      }
    });
});