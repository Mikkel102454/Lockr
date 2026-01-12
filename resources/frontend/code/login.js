(function(){
    function $(id){ return document.getElementById(id); }

    document.addEventListener('DOMContentLoaded', function(){
        if (window.applyBranding) window.applyBranding();

        const loginForm = $('login-form');
        const registerForm = $('register-form');
        const registerSubmit = $('register-submit');
        const loginSubmit = $('login-submit');

        function isValidEmail(email){
            return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email);
        }

        function validateRegisterForm(){
            const name = $('reg-name').value.trim();
            const email = $('reg-email').value.trim();
            const pw = $('reg-password').value || '';
            const ok = name.length > 0 && isValidEmail(email) && pw.length >= 8;
            if (registerSubmit) {
                registerSubmit.disabled = !ok;
                if (ok) registerSubmit.classList.add('btn-valid'); else registerSubmit.classList.remove('btn-valid');
            }
            return ok;
        }

        function showLogin(){
            loginForm.style.display = '';
            registerForm.style.display = 'none';
        }
        function showRegister(){
            registerForm.style.display = '';
            loginForm.style.display = 'none';
        }

        document.querySelectorAll('.register-link').forEach(el => {
            el.addEventListener('click', function(){
                showRegister();
            });
        });
        document.querySelectorAll('.login-link').forEach(el => {
            el.addEventListener('click', function(){
                showLogin();
            });
        });

        ['reg-name','reg-email','reg-password'].forEach(id => {
            const el = $(id);
            if (el) el.addEventListener('input', validateRegisterForm);
        });

        const origShowRegister = showRegister;
        showRegister = function(){
            origShowRegister();
            validateRegisterForm();
            const first = $('reg-name'); if (first) first.focus();
        };

        loginForm.addEventListener('submit', async function(e){
            e.preventDefault();
            const email = $('login-email').value.trim();
            const pw = $('login-password').value;

            const emailElementErr = $('login-email-err');

            if (!email || !pw) {
                alert('Please provide email and password');
                return;
            }

            const payload = {
                email: email,
                password: pw
            };

            const response = await fetch("/api/users/login", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "Accept": "application/json",
                },
                body: JSON.stringify(payload),
            });

            if(!response.ok){
                const result = await response.json();

                emailElementErr.innerText = result.message;
                return;
            }

            window.location.href = 'index.html';
        });

        registerForm.addEventListener('submit', async function(e){
            e.preventDefault();
            if (!validateRegisterForm()) {
                alert('Please complete the form correctly. Password must be at least 8 characters.');
                return;
            }
            const nameElement = $('reg-name');
            const emailElement = $('reg-email');
            const passwordElement = $('reg-password');

            const nameElementErr = $('reg-name-err');
            const emailElementErr = $('reg-email-err');
            const passwordElementErr = $('reg-password-err');

            const name = nameElement.value.trim();
            const email = emailElement.value.trim();
            const pw = passwordElement.value;

            const payload = {
                username: name,
                email: email,
                password: pw
            };

            const response = await fetch("/api/users/create", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "Accept": "application/json",
                },
                body: JSON.stringify(payload),
            });

            if(!response.ok){
                const result = await response.json();

                if(response.status === 409) {
                    switch (result.type) {
                        case "name":
                            nameElementErr.innerText = result.message;
                            emailElementErr.innerText = "";
                            passwordElementErr.innerText = "";
                            break;
                        case "email":
                            emailElementErr.innerText = result.message;
                            nameElementErr.innerText = "";
                            passwordElementErr.innerText = "";
                            break;
                        case "password":
                            passwordElementErr.innerText = result.message;
                            emailElementErr.innerText = "";
                            nameElementErr.innerText = "";
                            break;
                        default:
                            window.alert(result.message);
                            break;
                    }
                }

                return;
            }

            showLogin();
            registerForm.reset();
            validateRegisterForm();
        });
    });
})();
